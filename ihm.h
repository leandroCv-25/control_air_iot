#ifndef ihm_H_
#define ihm_H_


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class IHM {
private:
// #define NUMFLAKES 10  // Number of snowflakes in the animation example

// #define LOGO_HEIGHT 16
// #define LOGO_WIDTH 16
//   static const unsigned char PROGMEM logo_bmp[] = { B00000000, B11000000,
//                                                     B00000001, B11000000,
//                                                     B00000001, B11000000,
//                                                     B00000011, B11100000,
//                                                     B11110011, B11100000,
//                                                     B11111110, B11111000,
//                                                     B01111110, B11111111,
//                                                     B00110011, B10011111,
//                                                     B00011111, B11111100,
//                                                     B00001101, B01110000,
//                                                     B00011011, B10100000,
//                                                     B00111111, B11100000,
//                                                     B00111111, B11110000,
//                                                     B01111100, B11110000,
//                                                     B01110000, B01110000,
//                                                     B00000000, B00110000 };
  Adafruit_SSD1306 LcdOLED;
public:
  IHM();
  bool begin(int sda, int scl);
};



#endif