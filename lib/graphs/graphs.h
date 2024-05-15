/*
 * Drawing function for graphs
 * 
 * display  display class
 * x        x-coordinate
 * y        w-coordinate
 * w        graph width without ticks
 * h        graph height without ticks
 * Data     Data for line
 * Data2    Data for filled line (yellow)
 * Range    Set y-axis scale (autoscale = {0,0})
 * len      Number of elements in Data/Data2
 * title    Graph Title
 * 
 */

#include <GxEPD2_GFX.h>

void lineGraph(GxEPD2_GFX& display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float Data[], float Data2[], float Range[], int len, time_t starttime, String title, int line = 0x0000, int background = 0xFFFF, int highlight = 0xF800 );
