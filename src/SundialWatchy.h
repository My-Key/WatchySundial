#ifndef BOTWATCHY_H
#define BOTWATCHY_H

#include <Watchy.h>
#include "../include/images.h"
#include "../include/EMPORO8pt7b.h"
#include "../include/EMPORO10pt7b.h"
#include "../include/EMPORO12pt7b.h"

typedef struct Vector
{
  int x;
  int y;
}Vector;

class SundialWatchy : public Watchy
{
  using Watchy::Watchy;
public:
  void drawWatchFace();
  void drawTime();
  void drawDate();
  void drawBattery(int x, int y, uint16_t color, bool even);

  int getBatteryFill(int steps);

  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                              int16_t w, int16_t h, uint16_t color, bool even);
  
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color, bool even);
                                
  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color, bool even);
};

#endif
