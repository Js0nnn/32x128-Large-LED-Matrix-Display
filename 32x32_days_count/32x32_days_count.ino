
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"
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

char msg[50];

void setup() {
  Serial.begin(115200);

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
  P.setIntensity(5);
  P.displayClear();
}

void printdis() {

  P.synchZoneStart();
  P.displayAnimate();


  String str="3";

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
    //P.displayClear();
    
  }
}

void loop() {
  printdis();
}
