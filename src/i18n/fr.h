#ifndef EN_STRINGS_H
#define EN_STRINGS_H

#include <Arduino.h>

const char STR_TIME_FORMAT[] PROGMEM = "%H:%M";

const char STR_TEMPERATURE[] PROGMEM = "Température";
const char STR_HUMIDITY[] PROGMEM = "Humidité";
const char STR_PRESSURE[] PROGMEM = "Pression";
const char STR_WIND_SPEED_GUST[] PROGMEM = "Vent | Rafale";
const char STR_UV_INDEX[] PROGMEM = "Indice UV";
const char STR_AIR_QUALITY[] PROGMEM = "Qualité de l'air";
const char STR_VISIBILITY[] PROGMEM = "Visibilité";
const char STR_PRECIPITATION[] PROGMEM = "Précipitation";
const char STR_MIN[] PROGMEM = "Min";
const char STR_MAX[] PROGMEM = "Max";
const char STR_FEELS_LIKE[] PROGMEM = "T. ressentie";
const char STR_ALERT_START[] PROGMEM = "Commencer";
const char STR_ALERT_END[] PROGMEM = "Fin";

const char* STR_WEEKDAYS[7] PROGMEM = {
  "Dimanche",
  "Lundi",
  "Mardi",
  "Mercredi",
  "Jeudi",
  "Vendredi",
  "Samedi"
};
const char* STR_CARDINAL_DIRECTIONS [16] = { 
  "N", "NNE", "NE", "ENE", "E", "ESE",
  "SE", "SSE", "S", "SSO", "SO", "OSO",
  "O", "ONO", "NO", "NNO"
};

const char* STR_UV_INDEX_STEPS [5] = { 
  "Faible",
  "Modéré",
  "Élevé",
  "Très élevé",
  "Extrême"
};

const char* STR_AIR_QUALITY_STEPS [5] = { 
  "Bon",
  "Satisfaisant",
  "Moyen",
  "Mauvais",
  "Très mauvais"
};

#endif
