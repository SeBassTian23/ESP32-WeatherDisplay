/*
 * Drawing function for graphs
 */

#include "graphs.h"
#include "text.h"
#include <math.h> 

void lineGraph(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float Data[], float Data2[], float Range[], int len, time_t starttime, String title )
{
    float ymin;
    float ymax;
    int ticklines = 5;
    int auto_scale_margin = 0;

    int maxYscale = -10000;
    int minYscale =  10000;
    if (Range[0] == 0.0 && Range[1] == 0.0) {
        for (int i = 1; i < len; i++ ) {
            if (Data[i] >= maxYscale) maxYscale = Data[i];
            if (Data[i] <= minYscale) minYscale = Data[i];

            if (Data2[i] >= maxYscale) maxYscale = Data2[i];
            if (Data2[i] <= minYscale) minYscale = Data2[i];
        }
        maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
        ymax = round(maxYscale + 0.5);
        if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
        ymin = round(minYscale);
    }
    else {
        ymin = Range[0];
        ymax = Range[1];
    }

    float steps = (ymax-ymin)/(ticklines);

    Serial.println("Y-Min: " + String(ymin));
    Serial.println("Y-Max: " + String(ymax));
    Serial.println("Steps: " + String(steps));
    Serial.println("Ticks: " + String(ticklines));

    // Title
    display.setFont();
    textCenter(display, x + w / 2, y-10, title);
    
    // Draw y-axis tick markers and dashed lines
    for (byte i =0; i<ticklines+1; i++){
        display.setFont();
        textRight(display,x-2,y+((h/ticklines)*i-4), String( lrint(ymin+steps*(ticklines-i)) ) );
        if(i == 0)
            continue;
        display.drawLine(x+1,y+((h/ticklines)*i),x+w-1,y+((h/ticklines)*i),GxEPD_RED);
        bool blank = true;
        for(byte r = 0; r<w; r = r+3 ){
            if(blank){
                display.drawLine(x+r,y+((h/ticklines)*i),x+r+3,y+((h/ticklines)*i),GxEPD_WHITE);
                blank = false;
            }
            else{
                blank = true;
            }
        }
    }

    // x-Axis
    display.drawLine(x,y+h,x+w,y+h,GxEPD_BLACK);

    // y-Axis
    display.drawLine(x,y,x,y+h,GxEPD_BLACK);

    // Draw data line 1
    float x1 = x + 1;
    float y1 = y + (ymax - constrain(Data[0], ymin, ymax)) / (ymax - ymin) * h;
    float y3 = y + (ymax - constrain(Data2[0], ymin, ymax)) / (ymax - ymin) * h;
    if(y1 > y+h-1)
      y1 = y+h-1;
    if(y3 > y+h-1)
      y3 = y+h-1;
    float x2, y2, y4;
    for (int i = 1; i < len; i++ ) 
    {
      x2 = x + i * w / (len-1) - 1;
      y2 = y + (ymax - constrain(Data[i], ymin, ymax)) / (ymax - ymin) * h + 1;
      y4 = y + (ymax - constrain(Data2[i], ymin, ymax)) / (ymax - ymin) * h + 1;
      if(y2 > y+h-1)
        y2 = y+h-1;
      if(y4 > y+h-1)
        y4 = y+h-1;
      display.drawLine(x1, y3, x2, y4, GxEPD_RED);

      for(byte r = 0; r<ceil(w/len)+1; r++)
      {
        float m = (y4-y3)/(x2-x1);
        float b = y3-m*x1;
        display.drawLine(x1+r, y+h-1, x1+r, m*(x1+r)+b, GxEPD_RED);
      }

      display.drawLine(x1, y1, x2, y2, GxEPD_BLACK);

      x1 = x2;
      y1 = y2;
      y3 = y4;

    }

    // x-Axis ticks
    struct tm *lt;
    char buff[32];
    for (int i = 0; i <= 4; i++) {
      display.setCursor(x-4+(w/4)*i ,y+h+6);
      display.print(String(12*i)+"h");
      // time_t tt = starttime +(i*12*3600);
      // lt = localtime(&tt);
      // strftime(buff, 32, "%I%p",lt);
      // // strftime(buff, 32, "%H",lt);
      // if(i>0)
      // {
      //   textCenter(display,x-4+(w/4)*i ,y+h+6, String(buff));
      // }
    }
}
