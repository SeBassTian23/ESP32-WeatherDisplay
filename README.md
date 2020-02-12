# ESP32 - Weather Display

> 7.5 in three color eInk Display, showing the current weather and five day forecast from Dark Sky as well as indoor sensor measurements using an ESP32

![Weather Display](./img/weather-display.jpg)

The weather data is received using the [Dark Sky](https://darksky.net) API. You can get a developer account with 1000 API calls for free. The display is updated about every 30 min and is performing two API calls, one to receive the current data and daily forecast and one to receive the 48h hourly forecast. The connection is using the https protocol.

## Assembling the Frame

![Assembling the frame and setting up the electronics](./img/display-assembly.jpg)

1. Rectangular hole cut at the bottom of the frame for the display for the displays FPC cable.
2. Double sided sticky tape added to the corners of the display's backside.
3. The display glued into the center of the picture frame.
4. Inside, the FPC adapter is glued to the back with hot-glue.
5. Insert all the components into the back of the frame. Female connectors are glued into the frame, so the two boards (sensors & controller) can be easily attached with male pin connectors and be removed at any time. The battery is attached with velcro tape.
6. Cover the display with the plexiglas. To cover up the gap between the display and the frame, a vinyl frame was laser-cut from a vinyl sheet with a sticky back and glued onto the plexiglas.

## Wiring Components

The breakout boards are wired as shown in the schematic (still in the works).

## Hardware Components

+ **Picture Frame**
	+ Make sure to have enough room in the back for the electronics
+ **Controller (Driver)**
	+ [E-Paper ESP32 Driver Board](https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board)
+ **Display**
	+ [7.5inch e-Paper HAT (C)](https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT_(C)) - **V1, resolution 640x384**
+ **Sensors**
	+ Sparkfun [HTU21D - Humidity/Temperature](https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library) - *discontinued*
	+ Sparkfun [BME180 - Pressure/Temperature](https://github.com/sparkfun/BMP180_Breakout) - *discontinued*
	+ Adafruit [CSS811 - MOC/eCO2](https://github.com/adafruit/Adafruit_CCS811)
+ **Power**
	+ [Micro LiPo Charger](https://www.adafruit.com/product/1904)
	+ LiPO battery - can be one off the shelf or in my case I used an old laptop cell (3.7v @ 16.07Wh).

### Modifying the Controller

The power regulator on the controller board is not the best when using the board with battery, dropping out too early not making use of the batteries full capacity. It has been replaced with a [AP7361C-33ER-13](https://www.digikey.com/product-detail/en/diodes-incorporated/AP7361C-33ER-13/AP7361C-33ER-13DICT-ND/8545922) from Digi-Key, which has a lower dropout voltage (0.36V @ 1A).

## Software and Dependencies

+ **Display Driver and GFX**
	+ [Display Driver (Jean-Marc Zingg)](https://github.com/ZinggJM/GxEPD2)
+ **Fonts**
	+ [OpenSans](https://www.opensans.com) - Standard open font
	+ [GNU FreeFont](https://www.gnu.org/software/freefont/) - included in Adafruit's GFX library
+ **Other**
	+ [ArduinoJson](https://arduinojson.org) - Powerful JSON parser for Arduino
	+ [WiFi](https://www.arduino.cc/en/Reference/WiFi) - Arduino IDE
	+ [HTTPClient](https://www.arduino.cc/en/Tutorial/HttpClient) - Arduino IDE
	+ [WiFiClientSecure](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiClientSecure) - Arduino IDE
	+ [math](https://www.arduino.cc/en/Math/H) - Arduino IDE

## Inspiration for Code and Design

+ [David Bird (G6EJD)](https://github.com/G6EJD/ESP32-e-Paper-Weather-Display) - Fantastic display for weather data from [Open Weather Map](https://openweathermap.org)
+ [Weather Icons (Erik Flowers)](https://erikflowers.github.io/weather-icons) - Beautiful icon font for weather icons
