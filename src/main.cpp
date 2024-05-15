/*
 * Weather Display
 *
 * Controller (Driver):
 *  - ESP32: https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board
 *
 * Display:
 *  - Display: https://www.waveshare.com/wiki/7.5inch_e-Paper_HAT_(B)
 *  - Driver: https://github.com/ZinggJM/GxEPD2 (Jean-Marc Zingg) Version 1.2.3
 *
 * Other:
 *  - ArduinoJson: https://arduinojson.org/  V7.x or above
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

#include <Arduino.h>

// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 1

/* Display Dependencies */
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

/* Mapping of Waveshare ESP32 Driver Board */
GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));

/* Dependencies */
#include "configuration.h"  
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <WiFiClientSecure.h>
#include <math.h> 

/* Sensors */
#include "SparkFunHTU21D.h"
#include "Adafruit_CCS811.h"
#include "Adafruit_BMP280.h"

HTU21D humiditySensor;
Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp;

/* Power Management */
const int ADC_PIN = 34;
const int POWER_SWITCH_PIN = 4;
int ADC_VALUE = 0;
float voltage = 0.0; 

/* Fonts */
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
#include "fonts/OpenSans_Regular8pt7b.h"
#include "fonts/OpenSans_Regular14pt7b.h"
#include "fonts/OpenSans_Regular22pt7b.h"
#include "fonts/OpenSans_Regular34pt7b.h"
#include "fonts/OpenSans_Regular64pt7b.h"

/* Hourly 48h Forcast */
#include "hourly.h"
#define max_points 48

Hourly_type Hourly[max_points];

/* 5 Day Forcast */
#include "forecast.h"
#define max_days 5

Forecast_type Forcast[max_days];

/* Weather Alerts */
#include "alert.h"
#define max_alerts 3

Alert_type Alerts[max_alerts];

/* Text Formatting */
#include "text.h"

/* Icons */
#include "icons.h"

/* Graphs */
#include "graphs.h"

/* Calculate Cardinal Direction */
#include "cardinal_directions.h"

/* Include Language file */
#include "i18n/i18n.h"

/* Current Weather Parameters */
int           gmtOffset_sec;
time_t        currentTime;
const char*   summary;
const char*   summaryDay;
const char*   icon;
int           weatherID;
float         precipAccumulation;
float         precipProbability;
float         precipType;
float         temperature;
float         apparentTemperature;
float         humidity;
float         pressure;
float         windSpeed;
int           windDeg;
float         cloudCover;
float         visibility;
float         uvIndex;
int           aqi;
float         temperatureMin;
float         temperatureMax;
time_t        sunriseTime;
time_t        sunsetTime;
float         moonPhase;

/* Inside Sensor Parameters */
float         insideHumidity;
float         insideTemperature;
float         insidePressure;
float         insideVOC;
float         insideECO2;

/* Alerts Data */
bool          hasAlert = false;
time_t        alertStart;
time_t        alertEnd;
const char*   alertType;
const char*   alertTitle;

/* Colormode */
int CM_LINE = GxEPD_BLACK;
int CM_BACKGROUND = GxEPD_WHITE;
int CM_HIGHLIGHT = GxEPD_RED;

/* Declarations */
uint8_t StartWiFi();
void StopWiFi();
void drawBattery();
void espSLEEP();
bool httpsRequest(WiFiClient& client, int req);
void getWeatherData();
void dataToSerial();
void buildView();
bool parseOpenWeatherMap(String json, int req);
void getSensorData();
void powerDownSensors();
void drawSensorData();
void drawWeekDay();
void drawSunMoonPhase();
void drawCurrentIcon(uint16_t x, uint16_t y, uint16_t s, const char* i, int id);
void drawSummary();
void drawAlert();
void drawForcast();
void drawGraphs();
void drawValue(uint16_t x, uint16_t y, float value, String title, String unit);
String cardinalDirection(int deg);
void drawLocation();

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

  // Power up sensors
  pinMode(POWER_SWITCH_PIN, OUTPUT);
  digitalWrite(POWER_SWITCH_PIN, LOW);

  // Setup
  humiditySensor.begin();
  ccs.begin();
  bmp.begin();

  // Colormode
  if( strcmp(DISPLAY_COLOR_MODE, "dark") == 0 )
  {
    CM_LINE = GxEPD_WHITE;
    CM_BACKGROUND = GxEPD_BLACK;
    CM_HIGHLIGHT = GxEPD_RED;
  }

  if( strcmp(DISPLAY_COLOR_MODE, "bw") == 0 )
  {
    CM_LINE = GxEPD_BLACK;
    CM_BACKGROUND = GxEPD_WHITE;
    CM_HIGHLIGHT = GxEPD_BLACK;
  }

  // Get Data and Update Screen
  getWeatherData();
}

void loop()
{
  // put your main code here, to run repeatedly:
}

/* Start WiFi and Connect to Network */
uint8_t StartWiFi()
{
  Serial.print("\r\nConnecting to: "); Serial.println(String(WIFI_SSID));
  // IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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

/* Battery Voltage */
void drawBattery()
{
  ADC_VALUE = analogRead(ADC_PIN);
  Serial.print("ADC VALUE = ");
  Serial.println(ADC_VALUE);
  voltage = (ADC_VALUE * 3.3 ) / (4095.0);
  voltage = (ADC_VALUE / 4095.0) * 7.46;

  Serial.print("Voltage = ");
  Serial.println(voltage);
  if(ADC_VALUE < 2000)
  {
    iconBatteryEmpty(display);
  }
}

/* Sleep */
void espSLEEP()
{
  display.powerOff();
  powerDownSensors();
  long SleepTimer = DISPLAY_SLEEP_DURATION * 60;

  struct tm *lt = localtime(&currentTime);

  if(lt->tm_hour >= DISPLAY_POWER_SAVE_START || lt->tm_hour < DISPLAY_POWER_SAVE_END )
  {
    SleepTimer = 7200;

    if( lt->tm_hour < DISPLAY_POWER_SAVE_END && (lt->tm_hour + ( SleepTimer / 3600 )) > DISPLAY_POWER_SAVE_END  )
    {
        SleepTimer  =  (DISPLAY_POWER_SAVE_END * 3600) - ( (lt->tm_hour * 3600) + (lt->tm_min * 60) + lt->tm_sec );
        if( SleepTimer < (DISPLAY_SLEEP_DURATION * 60) )
        {
            SleepTimer = SleepTimer + DISPLAY_SLEEP_DURATION * 60;
        }
    }
  }

  if(SleepTimer < DISPLAY_SLEEP_DURATION * 60)
  {
    SleepTimer = DISPLAY_SLEEP_DURATION * 60;
  }
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
  String uri = "http://" + String(OWM_API_URL)
             + "/data/3.0/onecall?lat=" + String(LOCATION_LATITUDE) + "&lon=" + String(LOCATION_LONGITUDE) 
             + "&exclude=" + OWM_API_EXCLUDE 
             + "&units=" + I18N_UNITS 
             + "&lang=" + I18N_LANGUAGE 
             + "&appid=" + OWM_API_KEY;
  if(req == 2)
    uri = "http://" + String(OWM_API_URL)
      + "/data/3.0/onecall?lat=" + String(LOCATION_LATITUDE) + "&lon=" + String(LOCATION_LONGITUDE) 
      + "&exclude=" + OWM_API_EXCLUDE_HOURLY 
      + "&units=" + I18N_UNITS 
      + "&lang=" + I18N_LANGUAGE 
      + "&appid=" + OWM_API_KEY;
  
  if(req == 3)
      uri = "http://" + String(OWM_API_URL)
      + "/data/2.5/air_pollution?lat=" + String(LOCATION_LATITUDE) + "&lon=" + String(LOCATION_LONGITUDE) 
      + "&appid=" + OWM_API_KEY;

  Serial.println(uri);

  // http.begin(client, server, 443, uri);
  // http.begin(client, "api.openweathermap.org", 80, uri);
  http.begin(uri);
  http.setTimeout(30000);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!parseOpenWeatherMap(http.getString(), req ) )
        return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("HTTP Error: %s", http.errorToString(httpCode).c_str());
    Serial.println("");
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

/* Receive Data from Open Weather Maps */
void getWeatherData()
{
  if (StartWiFi() == WL_CONNECTED) {
    byte Attempts = 1;
    bool Weather = false, WeatherH = false, WeatherAQI = false;
    WiFiClientSecure client;   // wifi client object
    while ((Weather == false || WeatherH == false) && Attempts < 3) { // Try up-to 2 time for Weather and Forecast data
      if (WeatherAQI == false) WeatherAQI = httpsRequest(client,3);
      if (WeatherH == false) WeatherH = httpsRequest(client,2);
      if (Weather  == false) Weather = httpsRequest(client,1);
      Attempts++;     

      if (Weather && WeatherH && WeatherAQI)
      {
        Serial.println("Weather "+ String(Weather));
        Serial.println("Hourly: "+ String(WeatherH));
        Serial.println("AQI: "+ String(WeatherAQI));
        StopWiFi();     // Reduces power consumption
        getSensorData();
        dataToSerial();
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
    drawWeekDay();
    drawSunMoonPhase();
    drawSensorData();
    drawSummary();
    drawForcast();
    drawLocation();
    drawGraphs();
    drawAlert();
    drawBattery();
  }
  while (display.nextPage());
  espSLEEP();
}

/* Parse JSON from Open Weather Maps */
bool parseOpenWeatherMap(String json, int req)
{
  Serial.print(F("\nCreating object...and "));

  // allocate the JsonDocument
  // DynamicJsonDocument doc(40 * 1024);
  JsonDocument doc;
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json.c_str());
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  if(req == 1)
    {
    // Get timezone offset
    gmtOffset_sec        = doc["timezone_offset"].as<int>();

    // Current Weather
    currentTime          = doc["current"]["dt"].as<time_t>()+gmtOffset_sec;
    summary              = doc["current"]["weather"][0]["description"].as<const char*>();
    icon                 = doc["current"]["weather"][0]["icon"].as<const char*>();
    weatherID            = doc["current"]["weather"][0]["id"].as<int>();
    precipProbability    = doc["current"]["precipProbability"].as<float>();
    precipType           = doc["current"]["precipType"].as<float>();
    temperature          = doc["current"]["temp"].as<float>();
    apparentTemperature  = doc["current"]["feels_like"].as<float>();
    humidity             = doc["current"]["humidity"].as<float>();
    pressure             = doc["current"]["pressure"].as<float>();
    windSpeed            = doc["current"]["wind_speed"].as<float>();
    windDeg              = doc["current"]["wind_deg"].as<int>();
    cloudCover           = doc["current"]["clouds"].as<float>();
    uvIndex              = doc["current"]["uvi"].as<float>();
    visibility           = doc["current"]["visibility"].as<float>();
    summaryDay           = doc["daily"][0]["summary"].as<const char*>();
    temperatureMin       = doc["daily"][0]["temp"]["min"].as<float>();
    temperatureMax       = doc["daily"][0]["temp"]["max"].as<float>();

    if(doc["current"].containsKey("rain"))
      precipAccumulation   = doc["current"]["rain"]["1h"].as<float>();
    else
      precipAccumulation = 0.0;
                                                                                    
    sunriseTime          = doc["daily"][0]["sunrise"].as<time_t>()+gmtOffset_sec;
    sunsetTime           = doc["daily"][0]["sunset"].as<time_t>()+gmtOffset_sec;
    moonPhase            = doc["daily"][0]["moon_phase"].as<float>();
    
    // Alerts
    hasAlert = false;
    if(doc.containsKey("alerts"))
    {
      hasAlert = true;
      for(byte r = 0; r < max_alerts; r++){
        Alerts[r].start    = doc["alerts"][r]["start"].as<time_t>()+gmtOffset_sec;
        Alerts[r].end      = doc["alerts"][r]["end"].as<time_t>()+gmtOffset_sec;
        Alerts[r].title    = doc["alerts"][r]["event"].as<const char*>();
        if(doc["alerts"][r]["tags"].size() > 0)
          Alerts[r].tag      = doc["alerts"][r]["tags"][0].as<const char*>();
      }
    }

    // 5 Day Forcast
    for (byte r = 0; r < max_days; r++) {
      // r+1 to skip the current day for the 5 day forcast
      Forcast[r].time               = doc["daily"][r+1]["dt"].as<time_t>();
      Forcast[r].summary            = doc["daily"][r+1]["description"].as<const char*>();
      Forcast[r].icon               = doc["daily"][r+1]["weather"][0]["icon"].as<const char*>();
      Forcast[r].id                 = doc["daily"][r+1]["weather"][0]["id"].as<int>();
      Forcast[r].temperatureMin     = doc["daily"][r+1]["temp"]["min"].as<float>();
      Forcast[r].temperatureMax     = doc["daily"][r+1]["temp"]["max"].as<float>();
    }
  }
  else if(req == 2)
  {
    // Hourly
    for (byte r = 0; r < max_points; r++) {
      Hourly[r].temperature         = doc["hourly"][r]["temp"].as<float>();        
      Hourly[r].apparentTemperature = doc["hourly"][r]["feels_like"].as<float>();
      Hourly[r].humidity            = doc["hourly"][r]["humidity"].as<float>();           
      Hourly[r].precipProbability   = doc["hourly"][r]["pop"].as<float>();
    }
    precipProbability = Hourly[0].precipProbability;
  }
  else if(req == 3)
  {
    aqi = doc["list"][0]["main"]["aqi"].as<int>();
  }
  doc.clear();
  return true;
}

/* Print Data to Terminal */
void dataToSerial()
{
  Serial.println("Timezone Offset: "+String(gmtOffset_sec));
  Serial.println("Current Time: "+String(currentTime));
  Serial.println("summary: "+String(summary));
  Serial.println("icon: "+String(icon));
  Serial.println("precipProbability: "+String(precipProbability));
  Serial.println("precipType: "+String(precipType));
  Serial.println("temperature: "+String(temperature));
  Serial.println("apparentTemperature: "+String(apparentTemperature));
  Serial.println("humidity: "+String(humidity));
  Serial.println("pressure: "+String(pressure));
  Serial.println("windSpeed: "+String(windSpeed));
  Serial.println("cloudCover: "+String(cloudCover));
  Serial.println("uvIndex: "+String(uvIndex));
  Serial.println("summary day: "+String(summaryDay));
  Serial.println("temperatureMin: "+String(temperatureMin));
  Serial.println("temperatureMax: "+String(temperatureMax));
  Serial.println("precipAccumulation: "+String(precipAccumulation));
  Serial.println("sunriseTime: "+String(sunriseTime));
  Serial.println("sunsetTime: "+String(sunsetTime));
  Serial.println("moonPhase: "+String(moonPhase));
  Serial.println("AQI: "+String(aqi));
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

  if (strcmp(I18N_UNITS, "imperial") == 0)
    insideTemperature = 1.8 * insideTemperature + 32.0;
  if (strcmp(I18N_UNITS, "standard") == 0)
    insideTemperature += 273.15;
  Serial.println("Inside Temperature: " + String(insideTemperature));
  Serial.println("Inside Humidity: " + String(insideHumidity));

  insidePressure = bmp.readPressure() / 100;
  
  //bmp.seaLevelForAltitude( elevation, bmp.readPressure() * 0.01 ); // Pressure is given in Pa
  insidePressure /= pow(1.0 - (LOCATION_ELEVATION / 44330.0), 5.255);

  // We need the wait here for the CSS811 to start outputting
  // the first results. Perhaps we can average here in the future
  // for more stable results
  delay(20000);

  if(ccs.available()){
    if(!ccs.readData()){
      // CO2 in ppm
      insideECO2 = ccs.geteCO2();
      Serial.print("eCO2: ");
      Serial.println(insideECO2);
      
      // VOC
      insideVOC = ccs.getTVOC();
      Serial.print("VOC: ");
      Serial.println(insideVOC);
    }
  }
}

/* Cut Power from Sensors */
void powerDownSensors()
{
  digitalWrite(POWER_SWITCH_PIN, HIGH);
}

/* Show Indoor Data */
void drawSensorData()
{
  display.fillRect(20, 260, 152, 2, CM_HIGHLIGHT);

  // Inside House Icon
  struct tm *lt = localtime(&currentTime);

  if(lt->tm_hour >= DISPLAY_POWER_SAVE_START || lt->tm_hour <= DISPLAY_POWER_SAVE_END )
  {
    Serial.println("Night Mode");
    iconHouse(display, "night", CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  }
  else
  {
    Serial.println("Day Mode");
    iconHouse(display, "day", CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  }

  // Sensor data with units
  drawValue(20, 312, insideHumidity, STR_HUMIDITY, "%");
  if(strcmp(I18N_UNITS,"imperial") == 0)
    drawValue(110, 312, (insidePressure * 0.02953), STR_PRESSURE, "inHg");
  else
    drawValue(110, 312, insidePressure, STR_PRESSURE, "hPa");
  drawValue(20, 347, insideVOC, "VOC", "");
  drawValue(110, 347, insideECO2, "CO2", "ppm");

  // Display Inside Temperature
  int16_t  x1, y1;
  uint16_t w, h;
  display.setTextColor(CM_LINE);
  display.setFont(&OpenSans_Regular22pt7b);
  display.setCursor(110,300);
  display.print(String(lrint(insideTemperature)));
  display.getTextBounds(String(lrint(insideTemperature)), 110, 300, &x1, &y1, &w, &h);

  display.setFont(&OpenSans_Regular8pt7b);
  display.setCursor(110+w+4, 273+9);
  if(strcmp(I18N_UNITS, "metric") == 0)
    display.print("°C");
  if(strcmp(I18N_UNITS, "imperial") == 0)
    display.print("°F");
  if(strcmp(I18N_UNITS, "standard") == 0)
    display.print("K");
}

/* Dislay Day of the Month and Weekday */
void drawWeekDay()
{
  struct tm *lt = localtime(&currentTime);
  char buff[32];
  Serial.print(buff);
  display.setFont();
  display.fillScreen(CM_BACKGROUND);
  display.fillRect(0,0,192,192,CM_HIGHLIGHT);
  display.setTextColor(CM_LINE);
  
  if(strcmp(DISPLAY_COLOR_MODE, "dark") == 0)
    display.setTextColor(CM_BACKGROUND);

  if(strcmp(DISPLAY_COLOR_MODE, "bw") == 0)
    display.setTextColor(CM_BACKGROUND);

  // Day
  display.setFont(&OpenSans_Regular64pt7b);
  strftime(buff, 32, "%e",lt);
  if( (int)buff == 1)
    textCenter(display, 76, 120, buff); // correct for x-offset in the character 1
  else
    textCenter(display, 82, 120, buff);

  // Weekday
  display.setFont(&OpenSans_Regular14pt7b);
  textCenter(display, 92, 170, STR_WEEKDAYS[lt->tm_wday]);
}

/* Sun Rise/Set and Moon Phase */
void drawSunMoonPhase()
{
  // Sun Rise Icon
  if(strcmp(DISPLAY_COLOR_MODE, "bw") == 0)
    iconSunRise(display, 20, 215, "up", CM_LINE, CM_BACKGROUND, CM_BACKGROUND);
  else
    iconSunRise(display, 20, 215, "up", CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);


  // Sun Set Icon
  if(strcmp(DISPLAY_COLOR_MODE, "bw") == 0)
    iconSunRise(display, 20, 240, "down", CM_LINE, CM_BACKGROUND, CM_BACKGROUND);
  else
    iconSunRise(display, 20, 240, "down", CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);

  // Sunrise Time
  struct tm *lt = localtime(&sunriseTime);
  char buff[32];
  display.setTextColor(CM_LINE);
  display.setFont(&OpenSans_Regular8pt7b);
  strftime(buff, 32, STR_TIME_FORMAT, lt);
  display.setCursor(40, 218);
  display.print(buff);
  Serial.println(buff);

  // Sunset Time
  lt = localtime(&sunsetTime);
  display.setTextColor(CM_LINE);
  display.setFont(&OpenSans_Regular8pt7b);
  strftime(buff, 32, STR_TIME_FORMAT, lt);
  display.setCursor(40, 245);
  display.print(buff);
  Serial.println(buff);

  // Moon Icon
  if( strcmp(DISPLAY_COLOR_MODE, "bw") == 0)
    iconMoonPhase(display, 155, 224, 18, moonPhase, CM_LINE, CM_BACKGROUND, CM_BACKGROUND);
  else if( strcmp(DISPLAY_COLOR_MODE, "dark") == 0)
    iconMoonPhase(display, 155, 224, 18, moonPhase, CM_BACKGROUND, CM_BACKGROUND, CM_HIGHLIGHT);
  else
    iconMoonPhase(display, 155, 224, 18, moonPhase, CM_LINE, CM_LINE, CM_HIGHLIGHT);
}

/* Current Weather Icon */
void drawCurrentIcon(uint16_t x, uint16_t y, uint16_t s, const char* i, int id)
{
  // Clear Day
  if(String(i) == "01d")
    iconClearDay(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Clear Night
  else if(String(i) == "01n")
    iconClearNight(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Rain
  else if(String(i) == "10d" || String(i) == "09d") //
    iconRain(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Sleet
  else if(id == 611 || id == 612 || id == 613) //
    iconSleet(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Snow
  else if(String(i) == "13d") //
    iconSnow(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Wind
  else if(String(i) == "50d") //
    iconWind(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Fog
  else if(String(i) == "50d")
    iconFog(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Cloudy
  else if(String(i) == "04d" || String(i) == "04n")
    iconCloudy(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Cloudy Scatter
  else if(String(i) == "03d" || String(i) == "03n")
    iconCloudyScatter(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Cloudy Day
  else if(String(i) == "02d")
    iconCloudyDay(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Cloudy Night
  else if(String(i) == "02n")
    iconCloudyNight(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Hail
  else if(String(i) == "50d")
    iconHail(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Thunderstorm
  else if(String(i) == "11d")
    iconThunderstorm(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Tornado
  else if(id == 781)
    iconTornado(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  // Unknown Condition
  else{
    iconCloudy(display,x,y,s,CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  }
}

/* Current Weather Summary */
void drawSummary()
{
  drawCurrentIcon(193,0,152, icon, weatherID );

  display.setFont();
  textCenter(display, 269, 154, summary);

  int16_t  x0, y0;
  uint16_t w, h;
  int y1 = 15;
  int y2 = y1+85;
  int y3 = y2+35+3;

  int col1 = 364;
  int col2 = col1 + 86;
  int col3 = col2 + 86;

  display.setFont();
  display.setTextColor(CM_LINE);

  if(precipAccumulation > 0.0)
  {
    if(strcmp(I18N_UNITS, "imperial") == 0)
      textRight(display, 345, y1, String( ( precipAccumulation * 0.03937008 ), 2 ) + " in/h");
    else
      textRight(display, 345, y1, String( precipAccumulation ) + " mm/h");
  }
  
  display.drawRect(212,173, 408, 2, CM_HIGHLIGHT);  

  // display.setFont(&OpenSans_Regular8pt7b);
  // display.setTextColor(CM_LINE);
  // display.setCursor(212,162);
  // display.print(summaryDay);
  // textCenter(display, 416, 162, summaryDay);

  // Set the Current Temperature
  display.setTextColor(CM_LINE);
  display.setFont(&OpenSans_Regular34pt7b);

  display.setCursor(col1,y1+48);
  display.print(lrint(temperature));

  display.getTextBounds(String(lrint(temperature)), col1, y1+48, &x0, &y0, &w, &h);
  // display.drawCircle( col1+w+13, y1+3, 3, CM_LINE);
  
  display.setFont(&OpenSans_Regular8pt7b); 
  // display.setCursor( col1+w+18, y1+11 );
  display.setCursor( col1+w+8, y1+11 );
  if(strcmp(I18N_UNITS, "metric") == 0)
    display.print("°C");
  if(strcmp(I18N_UNITS, "imperial") == 0)
    display.print("°F");
  if(strcmp(I18N_UNITS, "standard") == 0)
    display.print("K");

  // Feels like
  display.setFont(&OpenSans_Regular8pt7b); 
  display.setCursor(col1,y1+68);
  display.print( String(STR_FEELS_LIKE) + " " + String(lrint(apparentTemperature)) + "°");

  display.setFont();
  display.setCursor(col3-38, y1 + 62);
  display.print(String(STR_MIN)+"/"+String(STR_MAX));

  // Min Temp
  int col3tmp = col3+18;
  display.setFont(&OpenSans_Regular8pt7b); 
  display.setCursor( col3tmp, y1+68 );
  display.print( String(lrint(temperatureMin)) + "°" );

  display.getTextBounds(String(lrint(temperatureMin)) + "°", col3tmp, y1+68, &x0, &y0, &w, &h);

  // Separator
  col3tmp += w + 8;
  display.drawLine( col3tmp, y1+68, col3tmp, y1+57, CM_LINE );

  // Max Temp
  col3tmp += 6;
  display.setCursor(col3tmp, y1+68);
  display.print(String(lrint(temperatureMax)) + "°" );

  // Add Humidity
  drawValue(col1, y2, humidity, STR_HUMIDITY, "%");

  // Add Pressure
  if(strcmp(I18N_UNITS,"imperial") == 0)
    drawValue(col2, y2, (pressure * 0.02953) , STR_PRESSURE, "inHg");
  else
    drawValue(col2, y2, pressure, STR_PRESSURE, "hPa");

  // Add Wind Speed Value
  if(strcmp(I18N_UNITS,"imperial") == 0)
    drawValue(col3, y2, (windSpeed/0.44704), STR_WIND_SPEED, "mph " + cardinalDirection(windDeg) );
  else
    drawValue(col3, y2, (windSpeed), STR_WIND_SPEED, "m/s " + cardinalDirection(windDeg));

  // Add Air Quality
  if(uvIndex <= 2.5)
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, STR_UV_INDEX_STEPS[0]);
  else if(uvIndex <= 5.5)
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, STR_UV_INDEX_STEPS[1]);
  else if(uvIndex <= 7.5)
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, STR_UV_INDEX_STEPS[2]);
  else if(uvIndex <= 10.5)
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, STR_UV_INDEX_STEPS[3]);
  else if(uvIndex > 10.5)
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, STR_UV_INDEX_STEPS[4]);
  else
    drawValue(col1, y3, uvIndex, STR_UV_INDEX, "");

  // Add Air Quality
  if(aqi == 1)
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, STR_AIR_QUALITY_STEPS[0]);
  if(aqi == 2)
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, STR_AIR_QUALITY_STEPS[1]);
  if(aqi == 3)
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, STR_AIR_QUALITY_STEPS[2]);
  if(aqi == 4)
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, STR_AIR_QUALITY_STEPS[3]);
  if(aqi == 5)
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, STR_AIR_QUALITY_STEPS[4]);
  else
    drawValue(col2, y3, aqi, STR_AIR_QUALITY, "");

  // Add Visibility Value
  int visibilityOffset = 0;
  if(strcmp(I18N_LANGUAGE, "nl") == 0)
    visibilityOffset += 5;
  if(strcmp(I18N_UNITS, "imperial") == 0)
    drawValue(col3+visibilityOffset, y3, (visibility/1609.344), STR_VISIBILITY, "mi");
  else
    drawValue(col3+visibilityOffset, y3, (visibility/1000), STR_VISIBILITY, "km");
}

/* Weather Alert */
void drawAlert()
{
  if (hasAlert)
  {
    // Outline    
    if(strcmp(DISPLAY_COLOR_MODE, "bw") == 0)
      display.drawRoundRect( 480, 34, 140, 30, 3, CM_LINE);
    else
      display.fillRoundRect( 480, 34, 140, 30, 3, CM_HIGHLIGHT);

    // Title
    // display.setFont();
    // display.setCursor(486, 37);
    // display.print(STR_ALERTS);

    // Content
    int yStart = 40;
    if( !Alerts[1].title )
      yStart += 6;
    for (byte r = 0; r < 2; r++) {
      if( Alerts[r].title )
      {
        display.setTextColor(CM_LINE);
        // display.setCursor(486, yStart + (r * 12) );
        // display.print( "\xDB " + String(Alerts[r].title) );
        textEllipsis( display, 486, yStart + (r * 12), 120, "\xDB " + String(Alerts[r].title) );
        display.setTextColor(CM_BACKGROUND);
        display.setCursor(486, yStart + (r * 12) );
        display.print( "!" );
      }
      // if( strcmp(Alerts[r].tag, "") != 0 )
      //   display.print( Alerts[r].tag );
      // else
      //   display.print( Alerts[r].title );
      // break;
    }
  }
}

/* Five Day Weather Forecast */
void drawForcast()
{
  int16_t  x0, y0;
  uint16_t w, h;
  int xpos = 210;
  int dist = 90;
  for (byte r = 0; r < max_days; r++) {
    display.setFont();
    struct tm *lt = localtime(&Forcast[r].time);
    // char buff[32];
    // strftime(buff, 32, "%A",lt);
    // textCenter(display,xpos+(dist*r)+25,185, String(buff));
    textCenter(display,xpos+(dist*r)+25,185, STR_WEEKDAYS[lt->tm_wday]);
    drawCurrentIcon(xpos+(dist*r), 190, 50, Forcast[r].icon, Forcast[r].id );
    // textCenter(display,x+(dist*r)+25,242, String(lrint(Forcast[r].temperatureMin)) + "|" + String(lrint(Forcast[r].temperatureMax)) );

    // Min Temp
    display.setFont(); 
    textRight(display, xpos+(dist*r)+22-5, 242, String(lrint(temperatureMin)) );
    display.drawCircle( xpos+(dist*r)+22-4, 242, 1.5, CM_LINE);

    // Separator
    display.drawLine( xpos+(dist*r)+22, 242, xpos+(dist*r)+22, 248, CM_LINE );

    // Max Temp
    display.setCursor(xpos+(dist*r)+22 + 4, 242);
    display.print(String(lrint(Forcast[r].temperatureMax)));
    display.getTextBounds(String(lrint(Forcast[r].temperatureMax)), xpos+(dist*r)+22 +4, 242, &x0, &y0, &w, &h);
    display.drawCircle( xpos+(dist*r)+22+w+6, 242, 1.5, CM_LINE);

  }
  display.drawRect(212,260, 408, 2, CM_HIGHLIGHT);  
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
  lineGraph(display, x1, 280, w, 80, temp, apptemp, range, max_points, currentTime, String(STR_TEMPERATURE) + " & " + String(STR_FEELS_LIKE), CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
  range[1] = 100;
  float hum [max_points] = {0};
  float prob [max_points] = {0};
  for(byte r = 0; r<max_points; r++ )
  {
    hum[r]  = Hourly[r].humidity;
    prob[r] = Hourly[r].precipProbability*100;
  }
  lineGraph(display, x2, 280, w, 80, hum, prob, range, max_points, currentTime, String(STR_HUMIDITY) + " & " + String(STR_PRECIPITATION), CM_LINE, CM_BACKGROUND, CM_HIGHLIGHT);
}

/* Value with Title */
void drawValue(uint16_t x, uint16_t y, float value, String title, String unit)
{
  int16_t  x0, y0;
  uint16_t w, h;

  // Title
  display.setFont();
  display.cp437(true);
  display.setTextColor(CM_LINE);
  display.setCursor(x,y);
  display.print(title);

  // Value
  display.setTextColor(CM_LINE);
  display.setFont(&OpenSans_Regular8pt7b);
  display.setCursor(x+5,y+23);
  if( title == STR_AIR_QUALITY)
  {
    w = -5;
  }
  else if( title == STR_PRESSURE && strcmp(I18N_UNITS, "imperial") == 0)
  {
    display.print( String( value, 1 ) );
    display.getTextBounds( String( value, 1 ), x+5, y+23, &x0, &y0, &w, &h);
  }
  else if( title == STR_VISIBILITY && value >= 10.0)
  {
    display.print( ">" + String( value, 0 ) );
    display.getTextBounds( ">" + String( value, 0 ), x+5, y+23, &x0, &y0, &w, &h);
  }
  else
  { 
    display.print(String(lrint(value)));
    display.getTextBounds(String(lrint(value)), x+5, y+23, &x0, &y0, &w, &h);
  }

  // Unit
  display.setFont();
  display.setCursor(x+5+w+6,y+17);
  display.print(unit);
}

/* Location */
void drawLocation()
{
  display.setFont(&OpenSans_Regular8pt7b); 
  textRight(display, 620, 26, LOCATION_NAME );
  // textRight(display, 620, 26, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  // textRight(display, 620, 46, "abcdefghijklmnopqrstuvwxyz" );
  // textRight(display, 620, 66, "0123456789" );
  // textRight(display, 620, 66, "°C °F" );
}

/* Get Cardinal Direction */
String cardinalDirection(int deg)
{
  deg %= 360;
  int direction = (int) ( (float) deg / 22.5);
  Serial.println("Cardinal Direction Selector: ");
  return String(STR_CARDINAL_DIRECTIONS[direction]);
}
