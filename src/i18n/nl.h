#ifndef EN_STRINGS_H
#define EN_STRINGS_H

#include <Arduino.h>

const char STR_TIME_FORMAT[] PROGMEM = "%H:%M";

const char STR_TEMPERATURE[] PROGMEM = "Temperatuur";
const char STR_HUMIDITY[] PROGMEM = "Vochtigheid";
const char STR_PRESSURE[] PROGMEM = "Luchtdruk";
const char STR_WIND_SPEED[] PROGMEM = "Windsnelheid";
const char STR_UV_INDEX[] PROGMEM = "UV-Index";
const char STR_AIR_QUALITY[] PROGMEM = "Luchtkwaliteit";
const char STR_VISIBILITY[] PROGMEM = "Zicht";
const char STR_PRECIPITATION[] PROGMEM = "Neerslag";
const char STR_MIN[] PROGMEM = "Min";
const char STR_MAX[] PROGMEM = "Max";
const char STR_FEELS_LIKE[] PROGMEM = "Voelt als";
const char STR_ALERTS[] PROGMEM = "Waarschuwing(en)";

const char* STR_WEEKDAYS[7] PROGMEM = {
  "Zondag",
  "Maandag",
  "Dinsdag",
  "Woensdag",
  "Donderdag",
  "Vrijdag",
  "Zaterdag"
};
const char* STR_CARDINAL_DIRECTIONS [16] = { 
  "N", "NNE", "NE", "ENE", "E", "EZE",
  "ZE", "ZZE", "Z", "ZZW", "ZW", "WZW",
  "W", "WNW", "NW", "NNW"
};

const char* STR_UV_INDEX_STEPS [5] = { 
  "Laag",
  "Gematigd",
  "Hoog",
  "Heel hoog",
  "Extreem"
};

const char* STR_AIR_QUALITY_STEPS [6] = { 
  "Uitstekend"
  "Redelijk,"
  "Slecht",
  "Ongezond",
  "Zeer ongezond",
  "Gevaarlijk"
};

#endif