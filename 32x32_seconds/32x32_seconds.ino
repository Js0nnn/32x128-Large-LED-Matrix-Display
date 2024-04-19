#include <ESP32Time.h>
#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"
#include "time.h"
#include "sntp.h"

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_ZONES 4
#define ZONE_SIZE 4
#define MAX_DEVICES (MAX_ZONES * ZONE_SIZE)

#define CLK_PIN 18   // or SCK
#define DATA_PIN 23  // or MOSI
#define CS_PIN 5     // or SS

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid = "Pixel_Pro";
const char* password = "Akari@123";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
//const long  gmtOffset_sec = 3600;
//const int   daylightOffset_sec = 3600;

const char* time_zone = "IST-5:30";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

ESP32Time rtc;
//ESP32Time rtc(3600);  // offset in seconds GMT+1

unsigned int prev_min = 0, prev_sec = 0;
char msg[50];
bool pass = true;

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}


void setup() {
  Serial.begin(115200);

  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  sntp_servermode_dhcp(1);  // (optional)

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagicaly.
   */
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
   * A more convenient approach to handle TimeZones with daylightOffset 
   * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  /*---------set with NTP---------------*/
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    rtc.setTimeStruct(timeinfo);
  }

  // initialise the LED display
  P.begin(MAX_ZONES);


  // Set up zones
  P.setZone(0, 0, ZONE_SIZE - 1);
  P.setZone(1, ZONE_SIZE, 2 * ZONE_SIZE - 1);
  P.setZone(2, 2 * ZONE_SIZE, 3 * ZONE_SIZE - 1);
  P.setZone(3, 3 * ZONE_SIZE, 4 * ZONE_SIZE - 1);


  P.setFont(0, ZONE1FONT);
  P.setFont(1, ZONE2FONT);
  P.setFont(2, ZONE3FONT);
  P.setFont(3, ZONE4FONT);
  P.setCharSpacing(0);  // double height --> double spacing
  P.setIntensity(0);
  P.displayClear();
}

void printdis(int sec) {

  P.synchZoneStart();
  P.displayAnimate();


  String str = "";

  if (sec <= 9) {
    str = "0" + String(sec);
  } else {
    str = String(sec);
  }
  strcpy(msg, str.c_str());

  //Serial.println(msg);

  if (P.getZoneStatus(0) && P.getZoneStatus(1) && P.getZoneStatus(2) && P.getZoneStatus(3)) {

    P.setFont(0, ZONE1FONT);
    P.setFont(1, ZONE2FONT);
    P.setFont(2, ZONE3FONT);
    P.setFont(3, ZONE4FONT);

    P.setCharSpacing(0);

    P.displayZoneText(0, msg, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(1, msg, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(2, msg, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    P.displayZoneText(3, msg, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);

    // synchronise the start
    P.displayClear();
  }
}

void loop() {
  if (pass == true) {

    int sec = rtc.getSecond();
    int min = rtc.getMinute();
    int day = rtc.getDay();
    int hour = rtc.getHour();

    int min_diff = prev_min - min;
    if (min_diff < 0) min_diff = min_diff * (-1);

    int sec_diff = prev_sec - sec;
    if (sec_diff < 0) sec_diff = sec_diff * (-1);

    if (WiFi.status() == WL_CONNECTED && (min_diff >= 30)) {


      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        rtc.setTimeStruct(timeinfo);
      }

      prev_min = min;
    }

    if (sec_diff >= 1) {
      Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns time with specified format
      // formating options  http://www.cplusplus.com/reference/ctime/strftime/


      prev_sec = sec;
    }

    day = day - 29;
    if (day < 0) day = day * (-1);

    if (day == 0) {
      hour = hour - 6;
      if (hour < 0) hour = hour * (-1);

      if (hour == 0) {
        min = min - 59;
        if (min < 0) min = min * (-1);

        if (min == 0) {
          sec = sec - 59;
          if (sec < 0) sec = sec * (-1);

          if (sec == 0) {
            printdis(sec);
            pass = false;
          }else{
            printdis(sec);
          }


        } else {
          printdis(min);
        }

      } else {
        sec = sec - 59;
        if (sec < 0) sec = sec * (-1);
        printdis(sec);
      }

    } else {
      sec = sec - 59;
      if (sec < 0) sec = sec * (-1);
      printdis(sec);
    }
  } else {
    printdis(0);
  }
}
