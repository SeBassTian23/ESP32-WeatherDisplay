#ifndef EN_STRINGS_H
#define EN_STRINGS_H

#include <Arduino.h>

const char STR_TIME_FORMAT[] PROGMEM = "%H:%M";

const char STR_TEMPERATURE[] PROGMEM = "Temperatur";
const char STR_HUMIDITY[] PROGMEM = "Luftfeuchte";
const char STR_PRESSURE[] PROGMEM = "Luftdruck";
const char STR_WIND_SPEED[] PROGMEM = "Wind";
const char STR_UV_INDEX[] PROGMEM = "UV-Index";
const char STR_AIR_QUALITY[] PROGMEM = "Luftqualit\x84t";
const char STR_VISIBILITY[] PROGMEM = "Sichtweite";
const char STR_PRECIPITATION[] PROGMEM = "Niederschlag";
const char STR_MIN[] PROGMEM = "Min";
const char STR_MAX[] PROGMEM = "Max";
const char STR_FEELS_LIKE[] PROGMEM = "Gef\x81hlt";
const char STR_ALERTS[] PROGMEM = "Warnung(en)";

const char* STR_WEEKDAYS[7] PROGMEM = {
  "Sonntag",
  "Montag",
  "Dienstag",
  "Mittwoch",
  "Donnerstag",
  "Freitag",
  "Samstag"
};
const char* STR_CARDINAL_DIRECTIONS [16] = { 
  "N", "NNO", "NO", "ONO", "O", "OSO",
  "SO", "SSO", "S", "SSW", "SW", "WSW",
  "W", "WNW", "NW", "NNW"
};

const char* STR_UV_INDEX_STEPS [5] = { 
  "Niedrig",
  "M\x84\xE1ig",
  "Hoch",
  "Sehr Hoch",
  "Extrem"
};

const char* STR_AIR_QUALITY_STEPS [5] = { 
  "Gut",
  "Befriedigend",
  "M\x84\xE1ig",
  "Schlecht",
  "Sehr schlecht"
};

#endif