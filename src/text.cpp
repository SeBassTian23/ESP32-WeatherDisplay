/*
 * Simple Text formatting functions
 */

#include "text.h"

void textCenter(GxEPD2_GFX& display, int x, int y, String text)
{
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  display.setCursor(x - w / 2, y);
  display.print(text);
}

void textRight(GxEPD2_GFX& display, int x, int y, String text)
{
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  display.setCursor(x - w, y);
  display.print(text);
}