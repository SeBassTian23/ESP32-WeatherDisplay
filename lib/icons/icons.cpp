/*
 * Drawing instructions for Weather icons
 */

#include "icons.h"

void iconCloud(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line, int background, int highlight)
{
  // top circle
  display.fillCircle(x, y, r, line);
  // left circle
  display.fillCircle(x-r*0.85, y+r*0.8, r*0.85, line);
  // right circle
  display.fillCircle(x+r*1.1, y+r*0.8, r*0.85, line);
  // rectangle
  display.fillRect(x-r*0.85, floor(y+r*0.8), (x+r*1.1)- (x-r*0.85), ceil(r*0.85), line );

  // top circle
  float offset = 0.8;
  display.fillCircle(x, y, r*offset, background);
  // left circle
  display.fillCircle(x-r*0.85, y+r*0.8, r*0.85*offset, background);
  // right circle
  display.fillCircle(x+r*1.1, y+r*0.8, r*0.85*offset, background);
  // rectangle
  display.fillRect(x-r*0.85, ceil(y+r*0.7), (x+r*1.1) - (x-r*0.85), ceil(r*offset), background );

}

void iconSun(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line, int background, int highlight)
{
  display.drawLine(x-r*1.75, y, x+r*1.75,y, line );
  display.drawLine(x, y-r*1.75, x, y+r*1.75, line );
  display.drawLine(x - r*1.25, y - r*1.25, x + r*1.25, y +r*1.25, line);
  display.drawLine(x - r*1.25, y + r*1.25, x + r*1.25, y - r*1.25, line);
  display.fillCircle(x, y, r*1.2, background);
  display.fillCircle(x, y, r, line);
  float offset = 0.9;
  display.fillCircle(x, y, r*offset, highlight);
}

void iconMoon(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, int line, int background, int highlight)
{
  float offset = 0.9;
  display.fillCircle(x, y, r, line);
  display.fillCircle(x, y, r*offset, highlight);
  display.fillCircle(x+r, y-r, r, line);
  display.fillCircle(x+r, y-r, r*offset, background);
  display.fillRect(x, y-r*2, r*2.5, r, background);
  display.fillRect(x+r+1, y-r, r*1.5, r*1.5, background);
}

void iconClearDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconSun(display, x+s/2, y+s/2, s/5, line, background, highlight );
}

void iconClearNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconMoon(display, x+s/2, y+s/2, s/5, line, background, highlight);
}

void iconRain(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, background);

  for (int i = 0; i <= s*0.1; i++)
  {
    display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, line);
    display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, line);
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, line);
  }
}

void iconSleet(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, background);

  for (int i = 0; i <= s*0.1; i++)
  {
    if(i < 1 || i > s*0.1 *0.5 )
    {
      display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, line);
      display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, line);
    }
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    if(i < s*0.16 * 0.5 || i > s*0.16 * 0.8 )
      display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, line);
  }
}

void iconSnow(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, background);

  display.fillCircle( x+s/2.75, y+s*0.7, s*0.02, line);
  display.fillCircle( x+s/1.75, y+s*0.7, s*0.02, line);

  display.fillCircle( x+s/2.75, y+s*0.8, s*0.02, line);
  display.fillCircle( x+s/1.75, y+s*0.8, s*0.02, line);

  display.fillCircle( x+s/2.15, y+s*0.65, s*0.02, line);
  display.fillCircle( x+s/2.15, y+s*0.75, s*0.02, line);
  display.fillCircle( x+s/2.15, y+s*0.85, s*0.02, line);

}

void iconWind(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  for (int i = 0; i <= s*0.7; i++)
  {
    if(i < s*0.6)
      display.fillCircle( x+s*0.15+i, y+s*0.4, s*0.02, line);
    if(i < s*0.5)
      display.fillCircle( x+s*0.1+i, y+s*0.5, s*0.02, line);
    if(i < s*0.2)
      display.fillCircle( x+s*0.7+i, y+s*0.5, s*0.02, line);
    if(i < s*0.6)
      display.fillCircle( x+s*0.2+i, y+s*0.6, s*0.02, line);
  }
}

void iconFog(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.1, y+s*0.55, s*0.75, s/5, background);

  for (int i = 0; i <= s*0.7; i++)
  {
    display.fillCircle( x+s*0.1+i, y+s*0.6, s*0.02, line);
    display.fillCircle( x+s*0.2+i, y+s*0.7, s*0.02, line);
    display.fillCircle( x+s*0.15+i, y+s*0.8, s*0.02, line);
  }
}

void iconCloudy(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+(s/4)*3, y+s/4, s/10, line, background, highlight);
  iconCloud(display, x+s/2.1, y+s/2.2, s/5, line, background, highlight);
}

void iconCloudyScatter(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.2, s/5, line, background, highlight);
}

void iconCloudyDay(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconSun(display, x+(s/3)*2, y+s/2.5, s/6, line, background, highlight);
  iconCloud(display, x+s/2.2, y+s/2.2, s/5, line, background, highlight);
}

void iconCloudyNight(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconMoon(display, x+(s/3)*2, y+s/3, s/6, line, background, highlight);
  iconCloud(display, x+s/2.2, y+s/2.2, s/5, line, background, highlight);
}

void iconHail(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, background);

  for (int i = 0; i <= s*0.1; i++)
  {
    if(i < s*0.1 *0.5 || i == s*0.1 )
    {
      display.fillCircle( x+s*0.4-i*0.5, y+s*0.65+i, s*0.02, line);
      display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, line);
    }
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    if(i < s*0.16 *0.7 || i == s*0.16 )
      display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, line);
  }
}

void iconThunderstorm(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  iconCloud(display, x+s/2.2, y+s/2.5, s/5, line, background, highlight);
  display.fillRect(x+s*0.275, y+s*0.6, s/2.5, s/5, background);

  for (int i = 0; i <= s*0.1; i++)
  {
    display.fillCircle( x+s*0.6-i*0.5, y+s*0.65+i, s*0.02, line);
  }
  for (int i = 0; i <= s*0.16; i++)
  {
    display.fillCircle( x+s*0.5-i*0.5, y+s*0.65+i, s*0.02, line);
  }
  display.fillTriangle( x+s*0.3, y+s*0.75, x+s*0.325, y+s*0.65, x+s*0.375, y+s*0.65, highlight);
  display.fillTriangle( x+s*0.3, y+s*0.75, x+s*0.4, y+s*0.7, x+s*0.33, y+s*0.7, highlight);
  display.fillTriangle( x+s*0.3, y+s*0.85, x+s*0.35, y+s*0.7, x+s*0.4, y+s*0.7, highlight);

}

void fillEllipsis(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c)
{
  for(int yi=-h; yi<=h; yi++) {
    for(int xi=-w; xi<=w; xi++) {
        if(xi*xi*h*h+yi*yi*w*w <= h*h*w*w)
            display.writePixel(x+xi, y+yi, c);
    }
  }
}

void iconTornado(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t s, int line, int background, int highlight)
{
  // 1
  fillEllipsis(display, x+s*0.33, y+s*0.7, s/12*1.2, s/18*1.2, line);
  fillEllipsis(display, x+s*0.33, y+s*0.7, s/12, s/18, background);
  // 2
  fillEllipsis(display, x+s*0.32, y+s*0.65, s/9*1.2, s/16*1.2, line);
  fillEllipsis(display, x+s*0.32, y+s*0.65, s/9, s/16, background);
  // 3
  fillEllipsis(display, x+s*0.35, y+s*0.55, s/7*1.2, s/12*1.2, line);
  fillEllipsis(display, x+s*0.35, y+s*0.55, s/7, s/12, background);
  // 4
  fillEllipsis(display, x+s*0.425, y+s*0.425, s/5*1.2, s/8*1.2, line);
  fillEllipsis(display, x+s*0.425, y+s*0.425, s/5, s/8, background);
  // 5
  fillEllipsis(display, x+s*0.5, y+s*0.3, s/4*1.2, s/7*1.2, line);
  fillEllipsis(display, x+s*0.5, y+s*0.3, s/4, s/7, background);
}

void iconSunRise(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
{
  uint16_t r = 7;

  // Horizontal
  display.drawLine(x - r*2 + 2, y, x + r*2 - 2, y, line);
  // Vertical
  display.drawLine(x, y - r*2 + 2, x, y, line);
  // Angle Top right
  display.drawLine(x - r*2 + 5, y - r*2 + 5, x, y, line);
  // Angle Top left
  display.drawLine(x, y, x + r*2 - 5, y - r*2 + 5, line);
  // Remove lines inside
  display.fillCircle(x, y, r + 1, background);
  // Empty inside
  display.fillCircle(x, y, r-1, highlight);
  display.drawCircle(x, y, r-1, line);
  // Overwrite the bottom
  display.fillRect(x - r, y + 4, r*2, r, background);
  
  // Arrow up
  if(direction == "up")
  {
    display.fillTriangle( x-r/2-1, y+r-2, x, y+r - 7, x+r/2+1, y+r-2, background);
    display.drawLine( x-r/2, y+r-2, x, y+r - 6, line);
    display.drawLine( x, y+r - 6, x+r/2, y+r-2, line);
  }
  // Arrow DOWN
  if(direction == "down")
  {
    display.drawLine( x-r/2, y+r-2, x, y+r + 2, line);
    display.drawLine( x, y+r + 2, x+r/2, y+r-2, line);
  }
  // Horizon line
  display.drawLine( x-r, y+r-2, x-r/2, y+r-2, line);
  display.drawLine( x+r/2, y+r-2, x+r, y+r-2, line);
}

// 0 = new moon
// 0.25 to a first quarter moon, 
// 0.5 to a full moon,
// 0.75 to a last quarter moon. 
void iconMoonPhase(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t r, float phase, int line, int background, int highlight)
{
  display.fillCircle(x, y, r, highlight);
  display.drawCircle(x, y, r, line);
  if(phase == 0)
    display.fillCircle(x, y, r, line);
  else if(phase > 0 && phase < 0.5)
  {
    for (int i=0; i<r+1; i++ )
    {
      float cx = sqrt( (r * r) - (i*i) );
      float c2 = cx * 2 * (1-(phase*2));
      display.drawLine(x-cx,y+i, x-cx+c2, y+i, line);
      display.drawLine(x-cx,y-i, x-cx+c2, y-i, line);
    }
  }
  else if(phase == 0.5)
    display.fillCircle(x, y, r, highlight);
  else
  {
    display.fillCircle(x, y, r, line);
    for (int i=0; i<r+1; i++ )
    {
      float cx = sqrt( (r * r) - (i*i) );
      float c2 = cx * 2 * ((1-phase)*2);
      display.drawLine(x-cx,y+i, x-cx+c2, y+i, highlight);
      display.drawLine(x-cx,y-i, x-cx+c2, y-i, highlight);
    }
  }

  // Add moon surface on top
  display.drawPixel(x-r+22, y-r+1, line);
  display.drawPixel(x-r+12, y-r+3, line);
  display.drawPixel(x-r+24, y-r+3, line);
  display.drawPixel(x-r+13, y-r+4, line);
  display.drawPixel(x-r+15, y-r+4, line);
  display.drawPixel(x-r+17, y-r+4, line);
  display.drawPixel(x-r+24, y-r+4, line);
  display.drawPixel(x-r+26, y-r+4, line);
  display.drawPixel(x-r+10, y-r+5, line);
  display.drawPixel(x-r+17, y-r+5, line);
  display.drawPixel(x-r+26, y-r+5, line);
  display.drawPixel(x-r+15, y-r+6, line);
  display.drawPixel(x-r+17, y-r+6, line);
  display.drawPixel(x-r+19, y-r+6, line);
  display.drawPixel(x-r+7, y-r+7, line);
  display.drawPixel(x-r+14, y-r+7, line);
  display.drawPixel(x-r+16, y-r+7, line);
  display.drawPixel(x-r+18, y-r+7, line);
  display.drawPixel(x-r+19, y-r+7, line);
  display.drawPixel(x-r+23, y-r+7, line);
  display.drawPixel(x-r+25, y-r+7, line);
  display.drawPixel(x-r+28, y-r+7, line);
  display.drawPixel(x-r+30, y-r+7, line);
  display.drawPixel(x-r+6, y-r+8, line);
  display.drawPixel(x-r+9, y-r+8, line);
  display.drawPixel(x-r+12, y-r+8, line);
  display.drawPixel(x-r+15, y-r+8, line);
  display.drawPixel(x-r+17, y-r+8, line);
  display.drawPixel(x-r+19, y-r+8, line);
  display.drawPixel(x-r+20, y-r+8, line);
  display.drawPixel(x-r+21, y-r+8, line);
  display.drawPixel(x-r+23, y-r+8, line);
  display.drawPixel(x-r+26, y-r+8, line);
  display.drawPixel(x-r+28, y-r+8, line);
  display.drawPixel(x-r+29, y-r+8, line);
  display.drawPixel(x-r+31, y-r+8, line);
  display.drawPixel(x-r+8, y-r+9, line);
  display.drawPixel(x-r+10, y-r+9, line);
  display.drawPixel(x-r+14, y-r+9, line);
  display.drawPixel(x-r+15, y-r+9, line);
  display.drawPixel(x-r+17, y-r+9, line);
  display.drawPixel(x-r+18, y-r+9, line);
  display.drawPixel(x-r+20, y-r+9, line);
  display.drawPixel(x-r+22, y-r+9, line);
  display.drawPixel(x-r+23, y-r+9, line);
  display.drawPixel(x-r+25, y-r+9, line);
  display.drawPixel(x-r+26, y-r+9, line);
  display.drawPixel(x-r+28, y-r+9, line);
  display.drawPixel(x-r+29, y-r+9, line);
  display.drawPixel(x-r+30, y-r+9, line);
  display.drawPixel(x-r+6, y-r+10, line);
  display.drawPixel(x-r+8, y-r+10, line);
  display.drawPixel(x-r+11, y-r+10, line);
  display.drawPixel(x-r+15, y-r+10, line);
  display.drawPixel(x-r+16, y-r+10, line);
  display.drawPixel(x-r+18, y-r+10, line);
  display.drawPixel(x-r+20, y-r+10, line);
  display.drawPixel(x-r+23, y-r+10, line);
  display.drawPixel(x-r+24, y-r+10, line);
  display.drawPixel(x-r+26, y-r+10, line);
  display.drawPixel(x-r+29, y-r+10, line);
  display.drawPixel(x-r+6, y-r+11, line);
  display.drawPixel(x-r+7, y-r+11, line);
  display.drawPixel(x-r+9, y-r+11, line);
  display.drawPixel(x-r+10, y-r+11, line);
  display.drawPixel(x-r+12, y-r+11, line);
  display.drawPixel(x-r+16, y-r+11, line);
  display.drawPixel(x-r+18, y-r+11, line);
  display.drawPixel(x-r+21, y-r+11, line);
  display.drawPixel(x-r+24, y-r+11, line);
  display.drawPixel(x-r+28, y-r+11, line);
  display.drawPixel(x-r+30, y-r+11, line);
  display.drawPixel(x-r+31, y-r+11, line);
  display.drawPixel(x-r+3, y-r+12, line);
  display.drawPixel(x-r+5, y-r+12, line);
  display.drawPixel(x-r+7, y-r+12, line);
  display.drawPixel(x-r+9, y-r+12, line);
  display.drawPixel(x-r+11, y-r+12, line);
  display.drawPixel(x-r+16, y-r+12, line);
  display.drawPixel(x-r+19, y-r+12, line);
  display.drawPixel(x-r+21, y-r+12, line);
  display.drawPixel(x-r+23, y-r+12, line);
  display.drawPixel(x-r+24, y-r+12, line);
  display.drawPixel(x-r+25, y-r+12, line);
  display.drawPixel(x-r+32, y-r+12, line);
  display.drawPixel(x-r+5, y-r+13, line);
  display.drawPixel(x-r+7, y-r+13, line);
  display.drawPixel(x-r+8, y-r+13, line);
  display.drawPixel(x-r+10, y-r+13, line);
  display.drawPixel(x-r+13, y-r+13, line);
  display.drawPixel(x-r+17, y-r+13, line);
  display.drawPixel(x-r+22, y-r+13, line);
  display.drawPixel(x-r+1, y-r+14, line);
  display.drawPixel(x-r+5, y-r+14, line);
  display.drawPixel(x-r+6, y-r+14, line);
  display.drawPixel(x-r+8, y-r+14, line);
  display.drawPixel(x-r+10, y-r+14, line);
  display.drawPixel(x-r+12, y-r+14, line);
  display.drawPixel(x-r+14, y-r+14, line);
  display.drawPixel(x-r+17, y-r+14, line);
  display.drawPixel(x-r+18, y-r+14, line);
  display.drawPixel(x-r+25, y-r+14, line);
  display.drawPixel(x-r+28, y-r+14, line);
  display.drawPixel(x-r+31, y-r+14, line);
  display.drawPixel(x-r+1, y-r+15, line);
  display.drawPixel(x-r+2, y-r+15, line);
  display.drawPixel(x-r+5, y-r+15, line);
  display.drawPixel(x-r+7, y-r+15, line);
  display.drawPixel(x-r+9, y-r+15, line);
  display.drawPixel(x-r+11, y-r+15, line);
  display.drawPixel(x-r+13, y-r+15, line);
  display.drawPixel(x-r+15, y-r+15, line);
  display.drawPixel(x-r+29, y-r+15, line);
  display.drawPixel(x-r+3, y-r+16, line);
  display.drawPixel(x-r+5, y-r+16, line);
  display.drawPixel(x-r+7, y-r+16, line);
  display.drawPixel(x-r+8, y-r+16, line);
  display.drawPixel(x-r+10, y-r+16, line);
  display.drawPixel(x-r+14, y-r+16, line);
  display.drawPixel(x-r+16, y-r+16, line);
  display.drawPixel(x-r+1, y-r+17, line);
  display.drawPixel(x-r+2, y-r+17, line);
  display.drawPixel(x-r+4, y-r+17, line);
  display.drawPixel(x-r+6, y-r+17, line);
  display.drawPixel(x-r+8, y-r+17, line);
  display.drawPixel(x-r+10, y-r+17, line);
  display.drawPixel(x-r+12, y-r+17, line);
  display.drawPixel(x-r+15, y-r+17, line);
  display.drawPixel(x-r+18, y-r+17, line);
  display.drawPixel(x-r+1, y-r+18, line);
  display.drawPixel(x-r+3, y-r+18, line);
  display.drawPixel(x-r+5, y-r+18, line);
  display.drawPixel(x-r+7, y-r+18, line);
  display.drawPixel(x-r+14, y-r+18, line);
  display.drawPixel(x-r+16, y-r+18, line);
  display.drawPixel(x-r+2, y-r+19, line);
  display.drawPixel(x-r+6, y-r+19, line);
  display.drawPixel(x-r+8, y-r+19, line);
  display.drawPixel(x-r+10, y-r+19, line);
  display.drawPixel(x-r+14, y-r+19, line);
  display.drawPixel(x-r+16, y-r+19, line);
  display.drawPixel(x-r+1, y-r+20, line);
  display.drawPixel(x-r+2, y-r+20, line);
  display.drawPixel(x-r+3, y-r+20, line);
  display.drawPixel(x-r+5, y-r+20, line);
  display.drawPixel(x-r+7, y-r+20, line);
  display.drawPixel(x-r+13, y-r+20, line);
  display.drawPixel(x-r+15, y-r+20, line);
  display.drawPixel(x-r+3, y-r+21, line);
  display.drawPixel(x-r+5, y-r+21, line);
  display.drawPixel(x-r+7, y-r+21, line);
  display.drawPixel(x-r+9, y-r+21, line);
  display.drawPixel(x-r+10, y-r+21, line);
  display.drawPixel(x-r+17, y-r+21, line);
  display.drawPixel(x-r+2, y-r+22, line);
  display.drawPixel(x-r+3, y-r+22, line);
  display.drawPixel(x-r+5, y-r+22, line);
  display.drawPixel(x-r+7, y-r+22, line);
  display.drawPixel(x-r+11, y-r+22, line);
  display.drawPixel(x-r+13, y-r+22, line);
  display.drawPixel(x-r+15, y-r+22, line);
  display.drawPixel(x-r+18, y-r+22, line);
  display.drawPixel(x-r+20, y-r+22, line);
  display.drawPixel(x-r+2, y-r+23, line);
  display.drawPixel(x-r+4, y-r+23, line);
  display.drawPixel(x-r+5, y-r+23, line);
  display.drawPixel(x-r+6, y-r+23, line);
  display.drawPixel(x-r+10, y-r+23, line);
  display.drawPixel(x-r+13, y-r+23, line);
  display.drawPixel(x-r+16, y-r+23, line);
  display.drawPixel(x-r+19, y-r+23, line);
  display.drawPixel(x-r+3, y-r+24, line);
  display.drawPixel(x-r+4, y-r+24, line);
  display.drawPixel(x-r+6, y-r+24, line);
  display.drawPixel(x-r+7, y-r+24, line);
  display.drawPixel(x-r+8, y-r+24, line);
  display.drawPixel(x-r+10, y-r+24, line);
  display.drawPixel(x-r+12, y-r+24, line);
  display.drawPixel(x-r+14, y-r+24, line);
  display.drawPixel(x-r+15, y-r+24, line);
  display.drawPixel(x-r+17, y-r+24, line);
  display.drawPixel(x-r+19, y-r+24, line);
  display.drawPixel(x-r+3, y-r+25, line);
  display.drawPixel(x-r+5, y-r+25, line);
  display.drawPixel(x-r+7, y-r+25, line);
  display.drawPixel(x-r+9, y-r+25, line);
  display.drawPixel(x-r+11, y-r+25, line);
  display.drawPixel(x-r+15, y-r+25, line);
  display.drawPixel(x-r+18, y-r+25, line);
  display.drawPixel(x-r+20, y-r+25, line);
  display.drawPixel(x-r+21, y-r+25, line);
  display.drawPixel(x-r+4, y-r+26, line);
  display.drawPixel(x-r+7, y-r+26, line);
  display.drawPixel(x-r+9, y-r+26, line);
  display.drawPixel(x-r+10, y-r+26, line);
  display.drawPixel(x-r+11, y-r+26, line);
  display.drawPixel(x-r+12, y-r+26, line);
  display.drawPixel(x-r+13, y-r+26, line);
  display.drawPixel(x-r+14, y-r+26, line);
  display.drawPixel(x-r+17, y-r+26, line);
  display.drawPixel(x-r+19, y-r+26, line);
  display.drawPixel(x-r+6, y-r+27, line);
  display.drawPixel(x-r+7, y-r+27, line);
  display.drawPixel(x-r+9, y-r+27, line);
  display.drawPixel(x-r+11, y-r+27, line);
  display.drawPixel(x-r+14, y-r+27, line);
  display.drawPixel(x-r+16, y-r+27, line);
  display.drawPixel(x-r+20, y-r+27, line);
  display.drawPixel(x-r+7, y-r+28, line);
  display.drawPixel(x-r+8, y-r+28, line);
  display.drawPixel(x-r+10, y-r+28, line);
  display.drawPixel(x-r+14, y-r+28, line);
  display.drawPixel(x-r+18, y-r+28, line);
  display.drawPixel(x-r+9, y-r+29, line);
  display.drawPixel(x-r+11, y-r+29, line);
  display.drawPixel(x-r+14, y-r+29, line);
  display.drawPixel(x-r+15, y-r+29, line);
  display.drawPixel(x-r+16, y-r+29, line);
  display.drawPixel(x-r+15, y-r+30, line);
  display.drawPixel(x-r+19, y-r+30, line);
  display.drawPixel(x-r+17, y-r+31, line);

}

void iconHouse(GxEPD2_GFX& display, String timeofday, int line, int background, int highlight)
{
  // house
  display.drawRect(22, 280, 20, 12, line);

  // door
  display.drawRect(26, 284, 5, 8, line);
  // door handle
  display.drawLine(26, 287, 28, 287, line);

  // window
  if( timeofday == "night")
    display.fillRect(33, 284, 7, 5, highlight); // Window light
  display.drawRect(33, 284, 7, 5, line);
  display.drawLine(36, 284, 36, 288, line);
  
  // roof
  display.drawLine(20, 281, 32, 272, line);
  display.drawLine(32, 272, 43, 281, line);

  display.drawLine(24, 280, 33, 273.5, line);
  display.drawLine(26.5, 280, 33.5, 275, line);
  display.drawLine(29, 280, 35, 275.5, line);

  // path and street
  display.drawLine(26, 292, 30, 296, line);
  display.drawLine(31, 292, 35, 296, line);

  display.drawLine(20, 296, 30, 296, line);
  display.drawLine(35, 296, 48, 296, line);

  // street light
  if( timeofday == "night")
  {
    display.fillCircle(46, 285, 2, highlight); // Street light
    display.fillTriangle(46, 283, 42, 294, 50, 294, highlight); // Street light
  }
  display.drawLine(46, 286, 46, 294, line);
  display.drawCircle(46, 285, 2, line);
  display.fillRect(45, 284, 3, 1, line);
  display.fillRect(45, 292, 3, 3, line);

  // mailbox
  display.fillRect(23, 293, 3, 1, line);
  display.drawLine(24, 293, 24, 294, line);
}

void iconBatteryEmpty( GxEPD2_GFX& display, int line, int background, int highlight)
{
  display.drawRect(5, 5, 12, 7, line);
  display.drawRect(4, 6, 1, 5, line);
  display.fillRect(13, 5, 3, 7, line);
}

// TODO:

// small-craft-advisory
// void iconSmallCraftAdvisory(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// hurricane
// void iconHurricane(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// gale-warning
// void iconGaleWarning(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// storm-warning
// void iconStormWarning(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// hurricane-warning
// void iconHurricaneWarning(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// fire
// void iconFire(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// flood
// void iconFlood(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// smog
// void iconSmog(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// smoke
// void iconSmoke(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// tsunami
// void iconSmoke(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// smoke
// void iconTsunami(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// thermometer
// void iconThermometer(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// dust
// void iconDust(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}

// warning-triangle
// void iconWarning(GxEPD2_GFX& display, uint16_t x, uint16_t y, String direction, int line, int background, int highlight)
// {}
