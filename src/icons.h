/*
 * Drawing instructions for Weather icons
 * Inspired by Weather Icons
 * https://erikflowers.github.io/weather-icons/
 */

#include <GxEPD2_GFX.h>

void iconCloud(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r);
void iconSun(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r);
void iconMoon(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r);
void iconClearDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconClearNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconRain(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconSleet(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconSnow(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconWind(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconFog(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconCloudy(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconCloudyDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconCloudyNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconHail(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconThunderstorm(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);
void iconTornado(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s);

void iconSunRise(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction);
void iconMoonPhase(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, float phase);
