# Remote control ATX power switch

##Prerequisites
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

### [mklittlefs](https://github.com/earlephilhower/mklittlefs)
This is for creating the filesystem binary
