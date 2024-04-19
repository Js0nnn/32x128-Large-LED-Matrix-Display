### README

#### ESP32 LED Display Clock

This repository contains code for an ESP32-based LED display clock. The clock utilizes the ESP32's capabilities to connect to WiFi and synchronize time with NTP servers. It displays the current time, day, and date on a LED display in a synchronized manner.

#### Components Used:
- ESP32 microcontroller
- LED display (MAX72xx)
- WiFi module

#### Setup Instructions:
1. Connect the ESP32 to the LED display according to the pin configuration specified in the code.
2. Modify the `ssid` and `password` variables in the code to match your WiFi network credentials.
3. Adjust the `ntpServer1` and `ntpServer2` variables if necessary to point to your preferred NTP servers.
4. Upload the code to your ESP32 board using the Arduino IDE or platform of your choice.
5. Once uploaded, power on the ESP32 board and it will connect to your WiFi network and synchronize time with NTP servers automatically.

#### File Structure:
- **`ESP32_LED_Clock.ino`**: The main Arduino sketch file responsible for setting up the clock, connecting to WiFi, and displaying time on the LED display.
- **`Font_Data.h`**: Header file containing font data used for displaying characters on the LED display.
- **`time.h`**: Header file providing time-related functions for the clock.
- **`sntp.h`**: Header file for Simple Network Time Protocol (SNTP) functions used to synchronize time with NTP servers.

#### Additional Notes:
- This code supports displaying time in hours, minutes, and days. Additional functionality or customization can be added as per requirements.
- Ensure that the pin numbers for CLK_PIN, DATA_PIN, and CS_PIN match your hardware setup.
- A similar project but with just 1 ESP32 microcontroller is under development.

Feel free to explore, modify, and adapt this code for your projects. If you have any questions or suggestions, please don't hesitate to reach out.

**Happy hacking!**
