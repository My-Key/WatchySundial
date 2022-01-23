#include "SundialWatchy.h"

const float VOLTAGE_MIN = 3.4;
const float VOLTAGE_MAX = 4.2;
const float VOLTAGE_RANGE = VOLTAGE_MAX - VOLTAGE_MIN;

const int TOP_TICK_SIZE = 26;
const int TICK_SIZE = 14;
const int SMALL_TICK = 4;

const Vector TOP_TICK_1 = {-TICK_SIZE / 2, 0};
const Vector TOP_TICK_2 = {TICK_SIZE / 2, 0};
const Vector TOP_TICK_3 = {0, TOP_TICK_SIZE};
const Vector TOP_TICK_4 = {0, TOP_TICK_SIZE / 2};

const Vector TICK_1 = {-TICK_SIZE / 2, 0};
const Vector TICK_2 = {TICK_SIZE / 2, 0};
const Vector TICK_3 = {0, TICK_SIZE};
const Vector TICK_4 = {0, TICK_SIZE / 2};

const Vector SMALL_TICK_1 = {-SMALL_TICK / 2, -SMALL_TICK / 2};
const Vector SMALL_TICK_2 = {SMALL_TICK / 2, SMALL_TICK / 2};
const Vector SMALL_TICK_3 = {-TICK_SIZE, TICK_SIZE};
const Vector SMALL_TICK_4 = {-TICK_SIZE / 2, TICK_SIZE / 2};

const Vector SMALL_TICK_POS_1 = {48,-84};
const Vector SMALL_TICK_POS_2 = {84,-48};

const int PIN_SIZE = 4;

const Vector PIN_1 = {-PIN_SIZE / 2, PIN_SIZE / 2};
const Vector PIN_2 = {PIN_SIZE / 2, PIN_SIZE / 2};
const Vector PIN_3 = {0, -PIN_SIZE / 2};

const int LENGTH_BACK_OFFSET = 10;
const int LENGTH_FORWARD_OFFSET = 18;
const int HOUR_HAND_WIDTH = 24;
const int HOUR_HAND_LENGTH = 35;

const Vector HOUR_HAND_1 = {-HOUR_HAND_WIDTH / 2, LENGTH_BACK_OFFSET};
const Vector HOUR_HAND_2 = {HOUR_HAND_WIDTH / 2, LENGTH_BACK_OFFSET};
const Vector HOUR_HAND_3 = {0, -HOUR_HAND_LENGTH - LENGTH_FORWARD_OFFSET};
const Vector HOUR_HAND_4 = {0, -HOUR_HAND_LENGTH - LENGTH_FORWARD_OFFSET + 25};

const int MINUTE_HAND_WIDTH = 12;
const int MINUTE_HAND_LENGTH = 75;
const int BORDER_WIDTH = 2;

const Vector MINUTE_HAND_START = {0, -HOUR_HAND_LENGTH};
const Vector MINUTE_HAND_1 = {-MINUTE_HAND_WIDTH / 2, LENGTH_BACK_OFFSET};
const Vector MINUTE_HAND_2 = {MINUTE_HAND_WIDTH / 2, LENGTH_BACK_OFFSET};
const Vector MINUTE_HAND_3 = {0, -MINUTE_HAND_LENGTH};
const Vector MINUTE_HAND_4 = {0, -MINUTE_HAND_LENGTH + 25};

const int BATTERY_LENGTH = MINUTE_HAND_1.y - MINUTE_HAND_4.y;

const double STEP_MINUTE = 360/60;
const double STEP_HOUR = 360/12;

const Vector CORNER_1 = {33, 99};
const Vector CORNER_2 = {99, 33};
const Vector CORNER_3 = {99, 99};
const Vector CORNER_4 = {80, 80};

const Vector SHADOW[] = {{0,-1}, {1,-1}, {1,-1}, {1,-1}, {2,0}, {2,0}, {-2,0}, {-2,0}, {-1,-1}, {-1,-1}, {-1,-1}, {0,-1}};

void SundialWatchy::drawWatchFace()
{
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_BLACK);


  Vector shadowOffset = SHADOW[currentTime.Hour %12];

  drawBitmap(0,0, epd_bitmap_BG, 200, 200, GxEPD_WHITE, true);
  drawBitmap(0 + shadowOffset.x, 0 + shadowOffset.y, epd_bitmap_FG, 200, 200, GxEPD_BLACK, true);
  //drawBitmap(-1,0, epd_bitmap_FG2, 200, 200, GxEPD_BLACK, false);
  //drawBitmap(-1,0, epd_bitmap_FG2, 200, 200, GxEPD_BLACK, true);
  display.drawBitmap(0,0, epd_bitmap_FG, 200, 200, GxEPD_WHITE);

  drawBitmap(24 + shadowOffset.x, 23 + shadowOffset.y, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK, true);
  //display.drawBitmap(24-1,23, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK);
  //display.drawBitmap(24+1,23, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK);
  //display.drawBitmap(24,23-1, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK);
  //display.drawBitmap(24,23+1, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK);

  display.drawBitmap(24,23, epd_bitmap_HourNumbers, 152, 81, GxEPD_BLACK);
  
  drawTime();

  display.drawBitmap(94,47, epd_bitmap_StyleMask, 12, 57, GxEPD_BLACK);
  display.drawBitmap(94 - shadowOffset.x,47 - shadowOffset.y, epd_bitmap_StyleMask, 12, 57, GxEPD_WHITE);


  display.drawBitmap(94,47, epd_bitmap_StyleShadow, 12, 57, GxEPD_BLACK);

  drawDate();

  drawBattery(100, 175, GxEPD_BLACK, true);
  drawBattery(100 + shadowOffset.x, 175 + shadowOffset.y, GxEPD_WHITE, true);
  drawBattery(100, 175, GxEPD_BLACK, false);
}

void SundialWatchy::drawBattery(int x, int y, uint16_t color, bool even)
{
  int batState = getBatteryFill(10);

  const int segmentWidth = 16;

  int startX = x - segmentWidth * batState / 2;

  drawBitmap(startX - 6, y, epd_bitmap_Sundial_battery_L, 6, 12, color, even);

  for (int i = 0; i < batState; i++)
    drawBitmap(startX + i * segmentWidth, y, epd_bitmap_Sundial_battery_mid, 16, 12, color, even);

  drawBitmap(100 + segmentWidth * batState / 2, y, epd_bitmap_Sundial_battery_R, 6, 12, color, even);
}

void SundialWatchy::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                              int16_t w, int16_t h, uint16_t color, bool even) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  display.startWrite();
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      if (byte & 0x80 && ((x + i + y) % 2 == 0) == even && x + i >= 0 && x + i < 200 && y >= 0 && y < 200)
        display.writePixel(x + i, y, color);
    }
  }
  display.endWrite();
}

static Vector rotateVector(Vector vector, double angle)
{
  double radians = angle * DEG_TO_RAD;
  double sinAngle = sin(radians);
  double cosAngle = cos(radians);

  Vector newVector = {0,0};
  newVector.x = cosAngle * (double)vector.x - sinAngle * (double)vector.y;
  newVector.y = sinAngle * (double)vector.x + cosAngle * (double)vector.y;
  return newVector;
}

static String numberToRoman(int number)
{
  String out = "";
  int num[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
  String sym[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
  int i=12;   

  while(number>0)
  {
    int div = number/num[i];
    number = number%num[i];

    while(div--)
    {
      out += sym[i];
    }

    i--;
  }

  return out;
}

void SundialWatchy::drawDate()
{
  display.setFont(&EMPORO8pt7b);

  String dayOfWeek = dayStr(currentTime.Wday);

  String month = monthStr(currentTime.Month);
  String date = month + " " + numberToRoman(currentTime.Day);
  
  int16_t  x1, y1;
  uint16_t w, h;
  display.getTextBounds(date, 0, 0, &x1, &y1, &w, &h);

  display.setCursor(100 - w / 2 , 135);
  display.print(date);
  
  display.setFont(&EMPORO12pt7b);
  
  display.getTextBounds(dayOfWeek, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(100 - w / 2, 160);
  display.println(dayOfWeek);
}

static double dotProduct(Vector v1, Vector v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

static double crossProduct(Vector v1, Vector v2)
{
  return v1.x * v2.y - v1.y * v2.x;
}

static Vector rotateVectorByRightAngle(Vector vector, int angle)
{
  angle = angle % 4;

  switch (angle)
  {
    case 1:
      return {-vector.y, vector.x};
    case 2:
      return {-vector.x, -vector.y};
    case 3:
      return {vector.y, -vector.x};
    default:
      return vector;
  }
}

int SundialWatchy::getBatteryFill(int steps)
{
  float VBAT = getBatteryVoltage();

  // 12 battery states
  int batState = int(((VBAT - VOLTAGE_MIN) / VOLTAGE_RANGE) * steps);
  if (batState > steps)
    batState = steps;
  if (batState < 0)
    batState = 0;

  return batState;
}

static Vector flip(Vector vector, bool negativeX, bool negativeY)
{
  if (negativeX)
    vector.x *= -1;

  if (negativeY)
    vector.y *= -1;

  return vector;
}

static double smoothstep(double x) {
  // Evaluate polynomial
  return x * x * (3 - 2 * x);
}

void SundialWatchy::drawTime()
{
  int hour = currentTime.Hour;
  int minute = currentTime.Minute;

  int minutes = ((hour + 6) % 12) * 60 + minute;
  double angle = minutes / 4.0;

  Vector hour1 = rotateVector({-100,0}, angle);
  double scale = smoothstep(abs((((hour + 6) % 12) * 60 + minute) / 360.0 - 1));

  hour1.x *= 0.7 + 0.3 * scale;
  hour1.y *= 0.7 + 0.3 * scale;

  hour1.x += 100;
  hour1.y += 100;

  fillTriangle(96,100, 96,86, hour1.x,hour1.y, GxEPD_BLACK, true);
  fillTriangle(96,86, 103,86, hour1.x,hour1.y, GxEPD_BLACK, true);
  fillTriangle(103,86, 103,100, hour1.x,hour1.y, GxEPD_BLACK, true);
}

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

void SundialWatchy::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color, bool even) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  display.startWrite();
  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    writeFastHLine(a, y0, b - a + 1, color, even);
    display.endWrite();
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    writeFastHLine(a, y, b - a + 1, color, even);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      _swap_int16_t(a, b);
    writeFastHLine(a, y, b - a + 1, color, even);
  }
  display.endWrite();
}

void SundialWatchy::writeFastHLine(int16_t x, int16_t y, int16_t w,
                                 uint16_t color, bool even) {
  display.startWrite();

  for (int i = 0; i < w; i++)
  {
    if (((x + i + y) % 2 == 0) == even)
      display.drawPixel(x + i, y, color);
  }

  display.endWrite();
}
