#Lamp

A homemade [dawn simulation](https://en.wikipedia.org/wiki/Dawn_simulation) lamp.
The time is kept using a real-time clock, coupled with an NTP connection to avoid drift.

##Material
- ESP8266-powered board (such as the [Wifi D1 mini](https://robotdyn.com/wifi-d1-mini-esp8266-dev-board-usb-ch340g.html) from RobotDyn)
- Real-time clock + SD card shield (such as the [Data logger shield](https://robotdyn.com/wifi-d1-mini-data-logger-shield-rtc-ds1307-with-battery-microsd.html), or two separate shields)
- Light dimmer (such as [this one](https://robotdyn.com/ac-light-dimmer-module-1-channel-3-3v-5v-logic-ac-50-60hz-220v-110v.html))
- Switches (1 temporary rocker and 1 on-on rocker)
- Light bulb + connectors

##Hardware installation
TODO

##Software installation
Pre-requisite: a [TimeZone database](https://timezonedb.com) API key is needed to convert from UTC to the local timezone

1. Install the [Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Install the [ESP8266 arduino extension](https://github.com/esp8266/Arduino)
    1. add `https://arduino.esp8266.com/stable/package_esp8266com_index.json` to the "Additional Boards Manager URLs" field in the IDE's preferences
    2. Install the extension via the "boards manager" of the IDE
3. Configure the board (`LOLIN(WEMOS) D1 R2 & mini` if using the one from the "material" section)
4. Compile and Upload to the board
5. Copy the `settings.ini` file to the SD card (and change the settings if needed)
6. Power up everything
