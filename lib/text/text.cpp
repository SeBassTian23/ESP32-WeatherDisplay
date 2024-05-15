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

void textEllipsis(GxEPD2_GFX& display, int x, int y, int width, String text)
{

  int16_t  x0, y0;
  uint16_t w, h;

  display.getTextBounds(String(text), x, y, &x0, &y0, &w, &h);

  uint16_t w_substr = w;
  String ellipsis = text;
  int length = ellipsis.length();
  if(w_substr > width){
    while(w_substr > width)
    {
      length -= 1;
      ellipsis = ellipsis.substring(0,length);
      display.getTextBounds(ellipsis, x, y, &x0, &y0, &w, &h);
      w_substr = w;
    }
    ellipsis = ellipsis.substring(0,length-3) + "...";
  }
  display.setCursor(x, y);
  display.print(ellipsis);
}