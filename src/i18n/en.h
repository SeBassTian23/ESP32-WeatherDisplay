#ifndef EN_STRINGS_H
#define EN_STRINGS_H

#include <Arduino.h>

const char STR_TIME_FORMAT[] PROGMEM = "%I:%M %p";

const char STR_TEMPERATURE[] PROGMEM = "Temperature";
const char STR_HUMIDITY[] PROGMEM = "Humidity";
const char STR_PRESSURE[] PROGMEM = "Pressure";
const char STR_WIND_SPEED_GUST[] PROGMEM = "Wind | Gust";
const char STR_UV_INDEX[] PROGMEM = "UV-Index";
const char STR_AIR_QUALITY[] PROGMEM = "Air Quality";
const char STR_VISIBILITY[] PROGMEM = "Visibility";
const char STR_PRECIPITATION[] PROGMEM = "Precipitation";
const char STR_MIN[] PROGMEM = "Min";
const char STR_MAX[] PROGMEM = "Max";
const char STR_FEELS_LIKE[] PROGMEM = "Feels Like";
const char STR_ALERT_START[] PROGMEM = "Start";
const char STR_ALERT_END[] PROGMEM = "End";

const char* STR_WEEKDAYS[7] PROGMEM = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};
const char* STR_CARDINAL_DIRECTIONS [16] PROGMEM = { 
  "N", "NNE", "NE", "ENE", "E", "ESE",
  "SE", "SSE", "S", "SSW", "SW", "WSW",
  "W", "WNW", "NW", "NNW"
};

const char* STR_UV_INDEX_STEPS [5] PROGMEM = { 
  "Low",
  "Moderate",
  "High",
  "Very High",
  "Extreme"
};

const char* STR_AIR_QUALITY_STEPS [5] PROGMEM = { 
  "Good",
  "Fair",
  "Moderate",
  "Poor",
  "Very Poor"
};

#endif
