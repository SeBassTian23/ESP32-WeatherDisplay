/*
 * Weather display
 * 
 * Version: see library.properties 
 * 
 * Controller (Driver):
 *  - ESP32: https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board
 * 
 * Display:
 *  - Display: https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT_(B)
 *  - Driver: https://github.com/ZinggJM/GxEPD2 (Jean-Marc Zingg)
 * 
 * Other:
 *  - ArduinoJson: https://arduinojson.org/  V6.x or above
 *  - WiFi: Arduino IDE
 *  - HTTPClient: Arduino IDE
 *  - WiFiClientSecure: Arduino IDE
 *  - math: Arduino IDE
 * 
 * Fonts:
 *  - OpenSans: https://www.opensans.com/
 * 
 * Icons (inspired by):
 *  - Weather Icons: https://erikflowers.github.io/weather-icons/
 * 
 * Sensors:
 *  - HTU21D: https://github.com/sparkfun/HTU21D_Breakout
 *  - BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
 *  - CSS811: https://github.com/adafruit/Adafruit_CCS811
 * 
 * Inspiration:
 *  - David Bird (G6EJD): https://github.com/G6EJD/ESP32-e-Paper-Weather-Display
 * 
 */

// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 1

/* Display Dependencies */
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>

/* Mapping of Waveshare ESP32 Driver Board */
GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));

/* Dependencies */
#include "credentials.h"  
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <math.h> 

/* Sensors */
#include "SparkFunHTU21D.h"
#include "Adafruit_CCS811.h"
#include "Adafruit_BMP280.h"

HTU21D humiditySensor;
Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp;

/* Fonts */
#include "fonts/OpenSans_Regular64pt7b.h"
#include "fonts/OpenSans_Regular14pt7b.h"
#include "fonts/OpenSans_Regular8pt7b.h"

/* Hourly 48h Forcast */
#include "src/hourly.h"
#define max_points 48

Hourly_type Hourly[max_points];

/* 5 Day Forcast */
#include "src/forecast.h"
#define max_days 5

Forecast_type Forcast[max_days];

/* Weather Alerts */
#include "src/alert.h"
#define max_alerts 3

Alert_type Alerts[max_alerts];

/* Text Formatting */
#include "src/text.h"

/* Icons */
#include "src/icons.h"

/* Graphs */
#include "src/graphs.h"

/* Current Weather Parameters */
time_t        currentTime;
const char*   summary;
const char*   summaryDay;
const char*   icon;
float         precipAccumulation;
float         precipProbability;
float         precipType;
float         temperature;
float         apparentTemperature;
float         humidity;
float         pressure;
float         windSpeed;
float         cloudCover;
float         uvIndex;
float         ozone;
float         temperatureMin;
float         temperatureMax;
float         sunriseTime;
float         sunsetTime;
float         moonPhase;

/* Inside Sensor Parameters */
float         insideHumidity;
float         insideTemperature;
float         insidePressure;
float         insideVOC;
float         insideECO2;

/* Alerts Data */
bool         hasAlert = false;
time_t       alertStart;
time_t       alertEnd;
const char*  alertType;
const char*  alertTitle;

/* Setup */
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init(115200);
  SPI.end();
  // Waveshare ESP32 Driver board
  // map and init SPI pins SCK(13), MISO(12), MOSI(14), SS(15)
  SPI.begin(13, 12, 14, 15);

  // Setup
  humiditySensor.begin();
  ccs.begin();
  bmp.begin();

  // Get Data and Update Screen
  getWeatherData();
  // buildView();
}

/* Program Loop */
void loop()
{
}

/* Start WiFi and Connect to Network */
uint8_t StartWiFi()
{
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  // IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  else Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}

/* Turn WiFi off */
void StopWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

/* Sleep */
void espSLEEP()
{
  display.powerOff();
  long SleepTimer = 1800; //30 min;
  esp_sleep_enable_timer_wakeup(SleepTimer * 1000000LL);

  Serial.println("Deep-sleep for " + String(SleepTimer) + " seconds");
  esp_deep_sleep_start();
}

/* Http Request */
bool httpsRequest(WiFiClient& client, int req)
{
  // close connection before sending a new request
  client.stop();
  HTTPClient http;
  String uri = "/forecast/" + apikey + "/" + longitude + "," + latitude + "?exclude=" + exclude + "&units=" + units;
  if(req == 2)
    uri = "/forecast/" + apikey + "/" + longitude + "," + latitude + "?exclude=" + excludeHour + "&units=" + units;
  http.begin(client, server, 443, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!parseDarkSky(http.getString(), req ) )
        return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

/* Receive Data from Dark Sky */
void getWeatherData()
{
  if (StartWiFi() == WL_CONNECTED) {
    byte Attempts = 1;
    bool Weather = false, WeatherH = false;
    WiFiClientSecure client;   // wifi client object
    while ((Weather == false || WeatherH == false) && Attempts < 2) { // Try up-to 2 time for Weather and Forecast data
      if (WeatherH == false) WeatherH = httpsRequest(client,2);
      if (Weather  == false) Weather = httpsRequest(client,1);
      Attempts++;     

      if (Weather && WeatherH)
      {
        Serial.println("Weather "+ String(Weather));
        Serial.println("Hourly: "+ String(WeatherH));
        StopWiFi();     // Reduces power consumption
        getSensorData();
        buildView();
      }
    }
    espSLEEP();
  }else{
    espSLEEP();
  }
}

/* Build Display View */
void buildView()
{
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do
  {
    weekdayDisplay();
    phasesSunMoon();
    sensorDisplay();
    drawSummary();
    drawForcast();
    drawGraphs();
    drawAlert();
  }
  while (display.nextPage());
  espSLEEP();
}

/* Parse JSON from Dark Sky */
bool parseDarkSky(String json, int req)
{
  Serial.print(F("\nCreating object...and "));

  // allocate the JsonDocument
  DynamicJsonDocument doc(40 * 1024);
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json.c_str());
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  Serial.println(doc.memoryUsage());
  // convert it to a JsonObject
  // JsonObject root = doc.as<JsonObject>();

  if(req == 1)
    {
    // Current Weather
    currentTime          = doc["currently"]["time"].as<time_t>()+gmtOffset_sec;     Serial.println("summary: "+String(currentTime));
    summary              = doc["currently"]["summary"].as<const char*>();           Serial.println("summary: "+String(summary));
    icon                 = doc["currently"]["icon"].as<const char*>();              Serial.println("icon: "+String(icon));
    precipProbability    = doc["currently"]["precipProbability"].as<float>();       Serial.println("precipProbability: "+String(precipProbability));
    precipType           = doc["currently"]["precipType"].as<float>();              Serial.println("precipType: "+String(precipType));
    temperature          = doc["currently"]["temperature"].as<float>();             Serial.println("temperature: "+String(temperature));
    apparentTemperature  = doc["currently"]["apparentTemperature"].as<float>();     Serial.println("apparentTemperature: "+String(apparentTemperature));
    humidity             = doc["currently"]["humidity"].as<float>();                Serial.println("humidity: "+String(humidity));
    pressure             = doc["currently"]["pressure"].as<float>();                Serial.println("pressure: "+String(pressure));
    windSpeed            = doc["currently"]["windSpeed"].as<float>();               Serial.println("windSpeed: "+String(windSpeed));
    cloudCover           = doc["currently"]["cloudCover"].as<float>();              Serial.println("cloudCover: "+String(cloudCover));
    uvIndex              = doc["currently"]["uvIndex"].as<float>();                 Serial.println("uvIndex: "+String(uvIndex));
    ozone                = doc["currently"]["ozone"].as<float>();                   Serial.println("ozone: "+String(ozone));
    summaryDay           = doc["daily"]["data"][0]["summary"].as<const char*>();    Serial.println("summary day: "+String(summary));
    temperatureMin       = doc["daily"]["data"][0]["temperatureMin"].as<float>();   Serial.println("temperatureMin: "+String(temperatureMin));
    temperatureMax       = doc["daily"]["data"][0]["temperatureMax"].as<float>();   Serial.println("temperatureMax: "+String(temperatureMax));
    if(doc["daily"]["data"][0].containsKey("precipAccumulation"))
      precipAccumulation   = doc["daily"]["data"][0]["precipAccumulation"].as<float>();
    else
      precipAccumulation = 0.0;
                                                                                    Serial.println("precipAccumulation: "+String(precipAccumulation));
    sunriseTime          = doc["daily"]["data"][0]["sunriseTime"].as<time_t>()+gmtOffset_sec;     Serial.println("sunriseTime: "+String(sunriseTime));
    sunsetTime           = doc["daily"]["data"][0]["sunsetTime"].as<time_t>()+gmtOffset_sec;      Serial.println("sunsetTime: "+String(sunsetTime));
    moonPhase            = doc["daily"]["data"][0]["moonPhase"].as<float>();        Serial.println("moonPhase: "+String(moonPhase));
    
    // Alerts
    hasAlert = false;
    if(doc.containsKey("alerts"))
    {
      hasAlert = true;
      for(byte r = 0; r < max_alerts; r++){
        Alerts[r].start    = doc["alerts"][r]["time"].as<time_t>()+gmtOffset_sec;
        Alerts[r].end      = doc["alerts"][r]["expires"].as<time_t>()+gmtOffset_sec;
        Alerts[r].severity = doc["alerts"][r]["severity"].as<const char*>();
        Alerts[r].title    = doc["alerts"][r]["title"].as<const char*>();
      }
    }

    // Forcast
    for (byte r = 0; r < max_days; r++) {
      Forcast[r].time               = doc["daily"]["data"][r+1]["time"].as<time_t>()+gmtOffset_sec;
      Forcast[r].summary            = doc["daily"]["data"][r+1]["summary"].as<const char*>();  
      Forcast[r].icon               = doc["daily"]["data"][r+1]["icon"].as<const char*>(); 
      Forcast[r].temperatureMin     = doc["daily"]["data"][r+1]["temperatureMin"].as<float>();
      Forcast[r].temperatureMax     = doc["daily"]["data"][r+1]["temperatureMax"].as<float>(); Serial.println("Forcast: "+String(Forcast[r].temperatureMax));
    }
  }
  else if(req == 2)
  {
    // Hourly
    for (byte r = 0; r < max_points; r++) {
      Hourly[r].temperature         = doc["hourly"]["data"][r]["temperature"].as<float>();        
      Hourly[r].apparentTemperature = doc["hourly"]["data"][r]["apparentTemperature"].as<float>();
      Hourly[r].humidity            = doc["hourly"]["data"][r]["humidity"].as<float>();           
      Hourly[r].precipProbability   = doc["hourly"]["data"][r]["precipProbability"].as<float>();
    } 
  }
  doc.clear();
  return true;
}

/* Get Readings from Sensor */
void getSensorData()
{
  insideHumidity = humiditySensor.readHumidity();
  insideTemperature = humiditySensor.readTemperature();
  // If temperature and Humidity are valid
  // The data can be used to set up the CSS811 sensor
  if( !isnan(insideHumidity) && !isnan(insideTemperature) )
  {
    ccs.setEnvironmentalData( (int) insideHumidity, insideTemperature );
  }

  if(units == "us")
    insideTemperature = 1.8 * insideTemperature + 32.0;
  Serial.println("Inside Temperature:" + String(insideTemperature));
  Serial.println("Inside Humidity:" + String(insideHumidity));

  bmp.readTemperature();
  insidePressure = bmp.seaLevelForAltitude( altitude, bmp.readPressure() * 0.01 ); // Pressure is given in Pa

  // We need the wait here for the CSS811 to start outputting
  // the first results. Perhaps we can average here in the future
  // for more stable results
  delay(20000);

  if(ccs.available()){
    if(!ccs.readData()){
      // CO2 in ppm
      insideECO2 = ccs.geteCO2();
      Serial.println(insideECO2);
      
      // CO2 in ppm     
      insideVOC = ccs.getTVOC();
      Serial.println(insideVOC);
    }
  }

  char status;
  double T,P,p0,a;

  status = pressureSensor.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressureSensor.getTemperature(T);
    if (status != 0)
    {
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      status = pressureSensor.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        status = pressureSensor.getPressure(P,T);
        if (status != 0)
        {
          insidePressure = pressureSensor.sealevel(P,altitude);
          Serial.println("Inside Pressure:" + String(insidePressure));
        }
      }
    }
  }
}

/* Show Indoor Data */
void sensorDisplay()
{
  display.fillRect(20, 260, 152, 2, GxEPD_RED);

  // Inside House Icon
  struct tm *lt = localtime(&currentTime);

  if(lt->tm_hour >= PowerSaveStart || lt->tm_hour <= PowerSaveEnd )
  {
    Serial.println("Night Mode");
    iconHouse(display, "night");
  }
  else
  {
    Serial.println("Day Mode");
    iconHouse(display, "day");
  }

  // Sensor data with units
  display.setTextColor(GxEPD_BLACK);
  display.setFont();
  // display.setCursor(60,270);
  // display.print("Temperature");
  display.setCursor(20,312);
  display.print("Humidity");
  display.setCursor(110,312);
  display.print("Pressure");
  display.setCursor(20,347);
  display.print("VOC");
  display.setCursor(110,347);
  display.print("CO2");
  
  int16_t  x1, y1;
  uint16_t w, h;

  // Display Inside Temperature
  display.setFont(&OpenSans_Regular22pt7b);
  display.setCursor(110,300);
  display.print(String(lrint(insideTemperature)));
  display.getTextBounds(String(lrint(insideTemperature)), 110, 300, &x1, &y1, &w, &h);
  display.drawCircle( 110+w+11, 273, 3, GxEPD_BLACK);
  // display.setFont();
  // display.setCursor(65+w+8,270);
  // display.print("F");

  // Display Inside Humidity
  display.setFont(&OpenSans_Regular8pt7b);
  display.setCursor(25,335);
  display.print(String(lrint(insideHumidity)));
  display.getTextBounds(String(lrint(insideHumidity)), 25, 335, &x1, &y1, &w, &h);
  display.setFont();
  display.setCursor(25+w+6,329);
  display.print("%");

  // Display Inside Pressure
  display.setFont(&OpenSans_Regular8pt7b);
  display.setCursor(115,335);
  display.print(String(lrint(insidePressure)));
  display.getTextBounds(String(lrint(insidePressure)), 115, 335, &x1, &y1, &w, &h);
  display.setFont();
  display.setCursor(115+w+6,329);
  display.print("hPa");

  display.setFont(&OpenSans_Regular8pt7b);
  // display.setCursor(25,335);
  // display.print(String(lrint(insideHumidity)));
  // display.setCursor(115,335);
  // display.print(String(lrint(insidePressure)));
  display.setCursor(25,370);
  display.print(String(lrint(insideVOC)));
  display.setCursor(115,370);
  display.print(String(lrint(insideECO2)));
  display.getTextBounds(String(lrint(insideECO2)), 115, 370, &x1, &y1, &w, &h);
  display.setFont();
  display.setCursor(115+w+6,364);
  display.print("ppm");
}

/* Dislay Day of the Month and Weekday */
void weekdayDisplay()
{
  struct tm *lt = localtime(&currentTime);
  char buff[32];
  Serial.print(buff);
  display.setFont();
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(0,0,192,192,GxEPD_RED);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&OpenSans_Regular64pt7b);
  strftime(buff, 32, "%e",lt);
  textCenter(display, 92, 120, String(buff));
  display.setFont(&OpenSans_Regular14pt7b);
  strftime(buff, 32, "%A",lt);
  textCenter(display, 92, 170, buff);
}

/* Sun Rise/Set and Moon Phase */
void phasesSunMoon()
{
  // Sun Rise Icon
  iconSunRise(display, 20, 215, "up");

  // Sun Set Icon
  iconSunRise(display, 20, 240, "down");

  // Sunrise Time
  struct tm *lt = localtime(&sunriseTime);
  char buff[32];
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&OpenSans_Regular8pt7b);
  strftime(buff, 32, "%I:%M %p",lt);
  display.setCursor(40, 218);
  display.print(buff);
  Serial.println(buff);

  // Sunset Time
  lt = localtime(&sunsetTime);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&OpenSans_Regular8pt7b);
  strftime(buff, 32, "%I:%M %p",lt);
  display.setCursor(40, 245);
  display.print(buff);
  Serial.println(buff);

  // Moon Icon
  iconMoonPhase(display, 155, 224, 18, moonPhase);
}

/* Current Weather Icon */
void drawCurrentIcon(uint16_t x, uint16_t y, uint16_t s, const char* i)
{
  if(String(i) == "clear-day")
    iconClearDay(display,x,y,s);
  else if(String(i) == "clear-night")
    iconClearNight(display,x,y,s);
  else if(String(i) == "rain")
    iconRain(display,x,y,s);
  else if(String(i) == "snow")
    iconSnow(display,x,y,s);
  else if(String(i) == "sleet")
    iconSleet(display,x,y,s);
  else if(String(i) == "wind")
    iconWind(display,x,y,s);
  else if(String(i) == "fog")
    iconFog(display,x,y,s);
  else if(String(i) == "cloudy")
    iconCloudy(display,x,y,s);
  else if(String(i) == "partly-cloudy-day")
    iconCloudyDay(display,x,y,s);
  else if(String(i) == "partly-cloudy-night")
    iconCloudyNight(display,x,y,s);
  else if(String(i) == "hail")
    iconHail(display,x,y,s);
  else if(String(i) == "thunderstorm")
    iconThunderstorm(display,x,y,s);
  else if(String(i) == "tornado")
    iconTornado(display,x,y,s);
  else{
    Serial.println("Unknown icon");
    iconCloudy(display,x,y,s);
  }
}

/* Current Weather Summary */
void drawSummary()
{
  Serial.println(icon);
  Serial.println(summary);
  Serial.println(summaryDay);
  drawCurrentIcon(193,0,152, icon);
  int16_t  x0, y0;
  uint16_t w, h;
  int y1 = 15;
  int y2 = y1+45;
  int y3 = y2+45;
  int y4 = y3+45;
  display.setFont();
  display.setCursor(212,y1);
  if(precipProbability > 0.0 && precipAccumulation == 0.0)
    display.print(String(lrint( precipProbability*100 ) ) + "%");
  if(precipAccumulation > 0.0){
    if(units == "us")
      display.print( String( precipAccumulation ) + "in");
    else
      display.print( String( precipAccumulation ) + "mm");
  }
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&OpenSans_Regular8pt7b);
  textCenter(display, 268,162, summary);

  display.drawRect(212,173, 408, 2, GxEPD_RED);  

  display.setTextColor(GxEPD_BLACK);
  display.setCursor(364,162);
  display.print(summaryDay);

  display.setFont();
  display.setCursor(364,y1);
  display.print("Min");
  display.setCursor(364,y2);
  display.print("Humidity");
  display.setCursor(364,y3);
  display.print("UV-Index");

  display.setCursor(578,y1);
  display.print("Max");
  display.setCursor(510,y2);
  display.print("Pressure");
  display.setCursor(510,y3);
  display.print("Ozone");

  display.setFont(&OpenSans_Regular22pt7b);
  display.setCursor(369,y1+30);

  // Set the Current Temperature
  textCenter(display, 482, y1+30, String(lrint(temperature)));
  display.getTextBounds(String(lrint(temperature)), 482, y1+30, &x0, &y0, &w, &h);
  display.drawCircle( 482+(w/2)+9, y1+3, 3, GxEPD_BLACK);

  // Set the Apparent Temperature
  display.setFont();
  display.setCursor(482+(w/2)+8,y1+24);
  display.print( String(lrint(apparentTemperature)) );

  // Add Humidity Value
  display.setFont(&OpenSans_Regular14pt7b);
  display.setCursor(369,y2+30);
  display.print(String(lrint(humidity*100)));
  display.getTextBounds(String(lrint(humidity*100)), 369, y2+30, &x0, &y0, &w, &h);
  display.setFont();
  display.setCursor(369+w+6,y2+24);
  display.print("%");

  // Add UV Index Value
  display.setFont(&OpenSans_Regular14pt7b);
  display.setCursor(369,y3+30);
  display.print(String(lrint(uvIndex)));

  // Minimum Temperature
  display.setFont(&OpenSans_Regular14pt7b);
  display.setCursor(369,y1+30);
  display.print(String(lrint(temperatureMin)));
  display.getTextBounds(String(lrint(temperatureMin)), 369, y1+30, &x0, &y0, &w, &h);
  display.drawCircle( 369+w+6, y1+15, 2, GxEPD_BLACK);

  // Maximum Temperature
  display.setFont(&OpenSans_Regular14pt7b);
  textRight(display, 610, y1+30, String(lrint(temperatureMax)));
  display.drawCircle( 610+6, y1+15, 2, GxEPD_BLACK);

  // Add Pressure Value
  display.setFont(&OpenSans_Regular14pt7b);
  display.setCursor(515,y2+30);
  display.print(String(lrint(pressure)));
  display.getTextBounds(String(lrint(pressure)), 515, y2+30, &x0, &y0, &w, &h);
  display.setFont();
  display.setCursor(515+w+6,y2+24);
  display.print("hPa");

  // Add Ozone Value
  display.setFont(&OpenSans_Regular14pt7b);
  display.setCursor(515,y3+30);
  display.print(String(lrint(ozone)));
  display.getTextBounds(String(lrint(ozone)), 515, y3+30, &x0, &y0, &w, &h);
  display.setFont();
  display.setCursor(515+w+6,y3+24);
  display.print("DU");
}

/* Weather Alert */
void drawAlert()
{
  if (hasAlert)
  {
    // std::string alertstring = "";
    for (byte r = 0; r < max_days; r++) {
      // alertstring = alertstring + String(Alerts[r].title) + " (" + String(Alerts[r].severity) +  ")";
    }
    //display.setFont();
    //display.fillRect(212,370, 384, 14, GxEPD_RED); 
    // alertStart
    // alertEnd
    //display.setTextColor(GxEPD_BLACK);
    //display.setCursor(232,375);
    // display.print(alertstring);
    //display.setFont();173
    display.fillRect(364,148, 275, 23, GxEPD_WHITE);
    display.setFont(&OpenSans_Regular8pt7b);
    display.setTextColor(GxEPD_BLACK);
    // display.setCursor(364,162);
    // display.print("! " + String(Alerts[0].title));

  
    display.setCursor(390,162);
    display.print(String(Alerts[0].title));

    display.fillTriangle(364, 165, 372, 145, 380, 165, GxEPD_RED);
    display.setCursor(371,162);
    display.setTextColor(GxEPD_BLACK);
    display.print("!");

  }
}

/* Five Day Weather Forecast */
void drawForcast()
{
  int x = 210;
  int dist = 90;
  for (byte r = 0; r < max_days; r++) {
    display.setFont();
    struct tm *lt = localtime(&Forcast[r].time);
    char buff[32];
    strftime(buff, 32, "%A",lt);
    textCenter(display,x+(dist*r)+25,185, String(buff));
    drawCurrentIcon(x+(dist*r),190,50, Forcast[r].icon);
    textCenter(display,x+(dist*r)+25,242, String(lrint(Forcast[r].temperatureMin)) + "/" + String(lrint(Forcast[r].temperatureMax)));
  }
  display.drawRect(212,260, 408, 2, GxEPD_RED);  
}

/* 48h Forecast Graphs */
void drawGraphs()
{
  int w = 160;
  int x1 = 240;
  int x2 = 640 - 30 - w;
  float temp [max_points] = {0};
  float apptemp [max_points] = {0};
  for(byte r = 0; r<max_points; r++ )
  {
    temp[r] = Hourly[r].temperature;
    apptemp[r] = Hourly[r].apparentTemperature;
  }
  float range [2] = {0,0};
  lineGraph(display, x1, 280, w, 80, temp, apptemp, range, max_points, currentTime, "Temp & Feels");
  range[1] = 100;
  float hum [max_points] = {0};
  float prob [max_points] = {0};
  for(byte r = 0; r<max_points; r++ )
  {
    hum[r]  = Hourly[r].humidity*100;
    prob[r] = Hourly[r].precipProbability*100;
  }
  lineGraph(display, x2, 280, w, 80, hum, prob, range, max_points, currentTime, "Hum. & Rain %");
}
