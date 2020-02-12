#ifndef FORECAST_H_
#define FORECAST_H_

  struct Forecast_type {
    time_t        time;
    const char*   summary;
    const char*   icon;
    float         temperatureMin;
    float         temperatureMax;
  };

#endif