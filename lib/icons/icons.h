/*
 * Drawing instructions for Weather icons
 * Inspired by Weather Icons
 * https://erikflowers.github.io/weather-icons/
 */

#ifndef ICONSHEADER_H
#define ICONSHEADER_H

#include <GxEPD2_GFX.h>

void iconCloud(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconSun(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconMoon(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconClearDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconClearNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconRain(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconSleet(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconSnow(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconWind(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconFog(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconCloudy(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconCloudyScatter(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconCloudyDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconCloudyNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconHail(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconThunderstorm(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconTornado(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);

void iconSunRise(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);
void iconMoonPhase(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, float phase, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);

void iconHouse(GxEPD2_GFX& display, String timeofday, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);

void iconBatteryEmpty( GxEPD2_GFX& display, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800);

#endif // ICONSHEADER_H