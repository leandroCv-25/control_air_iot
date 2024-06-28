#include "ihm.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)

// void testdrawline() {
//   int16_t i;

//   LcdOLED.clearDisplay(); // Clear display buffer

//   for(i=0; i<LcdOLED.width(); i+=4) {
//     LcdOLED.drawLine(0, 0, i, LcdOLED.height()-1, WHITE);
//     LcdOLED.display(); // Update screen with each newly-drawn line
//     delay(1);
//   }
//   for(i=0; i<LcdOLED.height(); i+=4) {
//     LcdOLED.drawLine(0, 0, LcdOLED.width()-1, i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   delay(250);

//   LcdOLED.clearDisplay();

//   for(i=0; i<LcdOLED.width(); i+=4) {
//     LcdOLED.drawLine(0, LcdOLED.height()-1, i, 0, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   for(i=LcdOLED.height()-1; i>=0; i-=4) {
//     LcdOLED.drawLine(0, LcdOLED.height()-1, LcdOLED.width()-1, i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   delay(250);

//   LcdOLED.clearDisplay();

//   for(i=LcdOLED.width()-1; i>=0; i-=4) {
//     LcdOLED.drawLine(LcdOLED.width()-1, LcdOLED.height()-1, i, 0, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   for(i=LcdOLED.height()-1; i>=0; i-=4) {
//     LcdOLED.drawLine(LcdOLED.width()-1, LcdOLED.height()-1, 0, i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   delay(250);

//   LcdOLED.clearDisplay();

//   for(i=0; i<LcdOLED.height(); i+=4) {
//     LcdOLED.drawLine(LcdOLED.width()-1, 0, 0, i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }
//   for(i=0; i<LcdOLED.width(); i+=4) {
//     LcdOLED.drawLine(LcdOLED.width()-1, 0, i, LcdOLED.height()-1, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000); // Pause for 2 seconds
// }

// void testdrawrect(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<LcdOLED.height()/2; i+=2) {
//     LcdOLED.drawRect(i, i, LcdOLED.width()-2*i, LcdOLED.height()-2*i, WHITE);
//     LcdOLED.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillrect(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<LcdOLED.height()/2; i+=3) {
//     // The INVERSE color is used so rectangles alternate white/black
//     LcdOLED.fillRect(i, i, LcdOLED.width()-i*2, LcdOLED.height()-i*2, INVERSE);
//     LcdOLED.display(); // Update screen with each newly-drawn rectangle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawcircle(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<max(LcdOLED.width(),LcdOLED.height())/2; i+=2) {
//     LcdOLED.drawCircle(LcdOLED.width()/2, LcdOLED.height()/2, i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillcircle(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=max(LcdOLED.width(),LcdOLED.height())/2; i>0; i-=3) {
//     // The INVERSE color is used so circles alternate white/black
//     LcdOLED.fillCircle(LcdOLED.width() / 2, LcdOLED.height() / 2, i, INVERSE);
//     LcdOLED.display(); // Update screen with each newly-drawn circle
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawroundrect(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<LcdOLED.height()/2-2; i+=2) {
//     LcdOLED.drawRoundRect(i, i, LcdOLED.width()-2*i, LcdOLED.height()-2*i,
//       LcdOLED.height()/4, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfillroundrect(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<LcdOLED.height()/2-2; i+=2) {
//     // The INVERSE color is used so round-rects alternate white/black
//     LcdOLED.fillRoundRect(i, i, LcdOLED.width()-2*i, LcdOLED.height()-2*i,
//       LcdOLED.height()/4, INVERSE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawtriangle(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=0; i<max(LcdOLED.width(),LcdOLED.height())/2; i+=5) {
//     LcdOLED.drawTriangle(
//       LcdOLED.width()/2  , LcdOLED.height()/2-i,
//       LcdOLED.width()/2-i, LcdOLED.height()/2+i,
//       LcdOLED.width()/2+i, LcdOLED.height()/2+i, WHITE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testfilltriangle(void) {
//   LcdOLED.clearDisplay();

//   for(int16_t i=max(LcdOLED.width(),LcdOLED.height())/2; i>0; i-=5) {
//     // The INVERSE color is used so triangles alternate white/black
//     LcdOLED.fillTriangle(
//       LcdOLED.width()/2  , LcdOLED.height()/2-i,
//       LcdOLED.width()/2-i, LcdOLED.height()/2+i,
//       LcdOLED.width()/2+i, LcdOLED.height()/2+i, INVERSE);
//     LcdOLED.display();
//     delay(1);
//   }

//   delay(2000);
// }

// void testdrawchar(void) {
//   LcdOLED.clearDisplay();

//   LcdOLED.setTextSize(1);      // Normal 1:1 pixel scale
//   LcdOLED.setTextColor(WHITE); // Draw white text
//   LcdOLED.setCursor(0, 0);     // Start at top-left corner
//   LcdOLED.cp437(true);         // Use full 256 char 'Code Page 437' font

//   // Not all the characters will fit on the LcdOLED. This is normal.
//   // Library will draw what it can and the rest will be clipped.
//   for(int16_t i=0; i<256; i++) {
//     if(i == '\n') LcdOLED.write(' ');
//     else          LcdOLED.write(i);
//   }

//   LcdOLED.display();
//   delay(2000);
// }

// void testdrawstyles(void) {
//   LcdOLED.clearDisplay();

//   LcdOLED.setTextSize(1);             // Normal 1:1 pixel scale
//   LcdOLED.setTextColor(WHITE);        // Draw white text
//   LcdOLED.setCursor(0,0);             // Start at top-left corner
//   LcdOLED.println(F("Hello, world!"));

//   LcdOLED.setTextColor(BLACK, WHITE); // Draw 'inverse' text
//   LcdOLED.println(3.141592);

//   LcdOLED.setTextSize(2);             // Draw 2X-scale text
//   LcdOLED.setTextColor(WHITE);
//   LcdOLED.print(F("0x")); LcdOLED.println(0xDEADBEEF, HEX);

//   LcdOLED.display();
//   delay(2000);
// }

// void testscrolltext(void) {
//   LcdOLED.clearDisplay();

//   LcdOLED.setTextSize(2); // Draw 2X-scale text
//   LcdOLED.setTextColor(WHITE);
//   LcdOLED.setCursor(10, 0);
//   LcdOLED.println(F("scroll"));
//   LcdOLED.display();      // Show initial text
//   delay(100);

//   // Scroll in various directions, pausing in-between:
//   LcdOLED.startscrollright(0x00, 0x0F);
//   delay(2000);
//   LcdOLED.stopscroll();
//   delay(1000);
//   LcdOLED.startscrollleft(0x00, 0x0F);
//   delay(2000);
//   LcdOLED.stopscroll();
//   delay(1000);
//   LcdOLED.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   LcdOLED.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   LcdOLED.stopscroll();
//   delay(1000);
// }

// void testdrawbitmap(void) {
//   LcdOLED.clearDisplay();

//   LcdOLED.drawBitmap(
//     (LcdOLED.width()  - LOGO_WIDTH ) / 2,
//     (LcdOLED.height() - LOGO_HEIGHT) / 2,
//     logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
//   LcdOLED.display();
//   delay(1000);
// }

// #define XPOS   0 // Indexes into the 'icons' array in function below
// #define YPOS   1
// #define DELTAY 2

// void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
//   int8_t f, icons[NUMFLAKES][3];

//   // Initialize 'snowflake' positions
//   for(f=0; f< NUMFLAKES; f++) {
//     icons[f][XPOS]   = random(1 - LOGO_WIDTH, LcdOLED.width());
//     icons[f][YPOS]   = -LOGO_HEIGHT;
//     icons[f][DELTAY] = random(1, 6);
//     Serial.print(F("x: "));
//     Serial.print(icons[f][XPOS], DEC);
//     Serial.print(F(" y: "));
//     Serial.print(icons[f][YPOS], DEC);
//     Serial.print(F(" dy: "));
//     Serial.println(icons[f][DELTAY], DEC);
//   }

//   for(;;) { // Loop forever...
//     LcdOLED.clearDisplay(); // Clear the display buffer

//     // Draw each snowflake:
//     for(f=0; f< NUMFLAKES; f++) {
//       LcdOLED.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
//     }

//     LcdOLED.display(); // Show the display buffer on the screen
//     delay(200);        // Pause for 1/10 second

//     // Then update coordinates of each flake...
//     for(f=0; f< NUMFLAKES; f++) {
//       icons[f][YPOS] += icons[f][DELTAY];
//       // If snowflake is off the bottom of the screen...
//       if (icons[f][YPOS] >= LcdOLED.height()) {
//         // Reinitialize to a random position, just off the top
//         icons[f][XPOS]   = random(1 - LOGO_WIDTH, LcdOLED.width());
//         icons[f][YPOS]   = -LOGO_HEIGHT;
//         icons[f][DELTAY] = random(1, 6);
//       }
//     }
//   }
// }

IHM::IHM(){
  
}

bool IHM::begin(int sda,int scl) {
  Wire.begin(sda,scl);
  LcdOLED =  Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!LcdOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // LcdOLED.display();

  // Clear the buffer
  LcdOLED.clearDisplay();
  delay(2000);  // Pause for 2 seconds

  // Draw a single pixel in white
  LcdOLED.drawPixel(10, 10, WHITE);
  LcdOLED.drawPixel(20, 10, WHITE);
  LcdOLED.drawPixel(10, 20, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  LcdOLED.display();
  delay(2000);
  // LcdOLED.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // LcdOLED.display(). These examples demonstrate both approaches...

  // testdrawline();  // Draw many lines

  // testdrawrect();  // Draw rectangles (outlines)

  // testfillrect();  // Draw rectangles (filled)

  // testdrawcircle();  // Draw circles (outlines)

  // testfillcircle();  // Draw circles (filled)

  // testdrawroundrect();  // Draw rounded rectangles (outlines)

  // testfillroundrect();  // Draw rounded rectangles (filled)

  // testdrawtriangle();  // Draw triangles (outlines)

  // testfilltriangle();  // Draw triangles (filled)

  // testdrawchar();  // Draw characters of the default font

  // testdrawstyles();  // Draw 'stylized' characters

  // testscrolltext();  // Draw scrolling text

  // testdrawbitmap();  // Draw a small bitmap image

  // // Invert and restore display, pausing in-between
  // LcdOLED.invertDisplay(true);
  // delay(1000);
  // LcdOLED.invertDisplay(false);
  // delay(1000);

  // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);  // Animate bitmaps
  return true;
}