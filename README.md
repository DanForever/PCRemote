# Remote control ATX power switch

An alternative, Internet of things (IoT) approach to Wake-on-Lan (WoL).

This is a device that you plug into a free USB2 motherboard header in order to draw power (assuming that your motherboard continues to provides power to the USB headers when turned off), and sits inbetween the motherboard front panel IO header and your case.
It will connect to your wifi, and you can access it using your web browser, and press a button to remote turn your PC on or off.

This was created for people for whom WoL isn't an option, but need to be able to check and control the power status of their PC remotely (such as people that work from home using some form of remote desktop software).

WoL works by broadcasting a special packet containing the mac address of the PC you want to turn on. Until your PC starts responding to normal requests, you don't actually know if the WoL packet worked or not, and you have no way of checking if your PC is functioning.
This device on the other hand, can tell you at a glance if your PC is on or off. There's less ambiguity involved.

## Prototype
This is the first prototype, made entirely using THT parts
![mk1](/docs/images/mk1.jpg)

## Prerequisites
### [ESPTool](https://github.com/espressif/esptool "Esptool")
This is for flashing the firmware and the filesystem to the microcontroller
### [Arduino-CLI](https://github.com/arduino/arduino-cli)
To compile the firmware
Also requires these libraries:
* [ESP8266WiFi](https://github.com/esp8266/Arduino)
 * [ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
  * [EEPROM](https://github.com/esp8266/Arduino/tree/master/libraries/EEPROM)
  * [LittleFS](https://github.com/esp8266/Arduino/tree/master/libraries/LittleFS)
* [ArduinoJson](https://arduinojson.org/)
* [StreamUtils](https://github.com/bblanchon/ArduinoStreamUtils)
* [DF-Print](https://github.com/DanForever/DF-Print)

### [mklittlefs](https://github.com/earlephilhower/mklittlefs)
This is for creating the filesystem binary
