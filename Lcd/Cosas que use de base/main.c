/**************************************************************************
  Interfacing PIC18F46K22 microcontroller with ILI9341 TFT display (240x320 pixel).
  C Code for CCS C compiler.
  This is a free software with NO WARRANTY.
  https://simple-circuit.com/
 
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
 
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
 
// define ILI9341 TFT module pin connections
#define TFT_CS    PIN_D0   // chip select pin
#define TFT_RST   PIN_D1   // reset pin, optional!
#define TFT_DC    PIN_D2   // data/command pin
 
#include <18F46K22.h>
#fuses  NOMCLR,NOLVP,NOBROWNOUT,PUT,NOXINST
#use delay(internal = 64MHz)
#use SPI(SPI1, MODE = 0, BITS = 8, STREAM = ILI9341)
 
#include <ILI9341.c>      // include ILI9341 TFT display driver
#include <GFX_Library.c>  // include graphics library
 
 
void testFillScreen() {
  display_fillScreen(ILI9341_BLACK);
  display_fillScreen(ILI9341_RED);
  display_fillScreen(ILI9341_GREEN);
  display_fillScreen(ILI9341_BLUE);
  display_fillScreen(ILI9341_BLACK);
}
 
void testText() {
  display_fillScreen(ILI9341_BLACK);
  display_setCursor(0, 0);
  display_setTextColor(ILI9341_WHITE);  display_setTextSize(1);
  display_print("Hello World!\r\n");
  display_setTextColor(ILI9341_YELLOW); display_setTextSize(2);
  printf(display_print, "%f\r\n", 1234.56);
  display_setTextColor(ILI9341_RED);    display_setTextSize(3);
  printf(display_print, "%LX\r\n", 0xDEADBEEF);
  display_print("\r\n");    // start a new line
  display_setTextColor(ILI9341_GREEN);
  display_setTextSize(5);
  display_print("Groop\r\n");
  display_setTextSize(2);
  display_print("I implore thee,\r\n");
  display_setTextSize(1);
  display_print("my foonting turlingdromes.\r\n");
  display_print("And hooptiously drangle me\r\n");
  display_print("with crinkly bindlewurdles,\r\n");
  display_print("Or I will rend thee\r\n");
  display_print("in the gobberwarts\r\n");
  display_print("with my blurglecruncheon,\r\n");
  display_print("see if I don't!\r\n");
}
 
void testLines(uint16_t color) {
  int16_t x1, y1, x2, y2,
          w = display_width,
          h = display_height;
 
  display_fillScreen(ILI9341_BLACK);
 
  x1 = y1 = 0;
  y2    = h - 1;
 
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);
 
  display_fillScreen(ILI9341_BLACK);
 
  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
 
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);
 
  display_fillScreen(ILI9341_BLACK);
 
  x1    = 0;
  y1    = h - 1;
  y2    = 0;
 
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);
 
  display_fillScreen(ILI9341_BLACK);
 
  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
 
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);
}
 
void testFastLines(uint16_t color1, uint16_t color2) {
  int16_t x, y, w = display_width, h = display_height;
 
  display_fillScreen(ILI9341_BLACK);
 
  for(y=0; y<h; y+=5) display_drawHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) display_drawVLine(x, 0, h, color2);
}
 
void testRects(uint16_t color) {
  int16_t n, i, i2,
          cx = display_width  / 2,
          cy = display_height / 2;
 
  display_fillScreen(ILI9341_BLACK);
  n = min(display_width, display_height);
 
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    display_drawRect(cx-i2, cy-i2, i, i, color);
  }
}
 
void testFilledRects(uint16_t color1, uint16_t color2) {
  int16_t n, i, i2,
          cx = display_width  / 2 - 1,
          cy = display_height / 2 - 1;
 
  display_fillScreen(ILI9341_BLACK);
  n = min(display_width, display_height);
  for(i=n; i>0; i-=6) {
    i2 = i / 2 - 1;
 
    display_fillRect(cx-i2, cy-i2, i, i, color1);
 
    // Outlines are not included in timing results
    display_drawRect(cx-i2, cy-i2, i, i, color2);
  }
}
 
void testFilledCircles(uint8_t radius, uint16_t color) {
  int16_t x, y, w = display_width, h = display_height, r2 = radius * 2;
 
  display_fillScreen(ILI9341_BLACK);
 
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      display_fillCircle(x, y, radius, color);
    }
  }
}
 
void testCircles(uint8_t radius, uint16_t color) {
  int16_t x, y, r2 = radius * 2,
          w = display_width  + radius,
          h = display_height + radius;
 
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      display_drawCircle(x, y, radius, color);
    }
  }
}
 
void testTriangles() {
  int16_t n, i, cx = display_width  / 2 - 1,
          cy = display_height / 2 - 1;
  uint16_t color = 0xF600;
  display_fillScreen(ILI9341_BLACK);
  n = min(cx, cy);
 
  for(i=0; i<n; i+=5) {
    display_drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      color);
    color += 100;
  }
}
 
void testFilledTriangles() {
  int16_t i, cx = display_width  / 2 - 1,
             cy = display_height / 2 - 1;
 
  display_fillScreen(ILI9341_BLACK);
 
  for(i=min(cx,cy); i>10; i-=5) {
 
    display_fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display_color565(0, i*2, i*2));
 
    display_drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display_color565(i*2, i*2, 0));
 
  }
}
 
void testRoundRects() {
  int16_t w, i, i2,
          cx = display_width  / 2 - 1,
          cy = display_height / 2 - 1;
 
  display_fillScreen(ILI9341_BLACK);
  w = min(display_width, display_height);
 
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    display_drawRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(i, 0, 0));
  }
}
 
void testFilledRoundRects() {
  int16_t i, i2,
          cx = display_width  / 2 - 1,
          cy = display_height / 2 - 1;
 
  display_fillScreen(ILI9341_BLACK);
 
  for(i=min(display_width, display_height); i>20; i-=6) {
    i2 = i / 2;
    display_fillRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(0, i, 0));
  }
}
 
// main function
void main(void)
{
  // initialize ILI9341 TFT
  tft_begin();
  // init done!
 
  delay_ms(10);
  testFillScreen();
  delay_ms(500);
 
  testText();
  delay_ms(3000);
 
  testLines(ILI9341_CYAN);
  delay_ms(500);
 
  testFastLines(ILI9341_RED, ILI9341_BLUE);
  delay_ms(500);
 
  testRects(ILI9341_GREEN);
  delay_ms(500);
 
  testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA);
  delay_ms(500);
 
  testFilledCircles(10, ILI9341_MAGENTA);
 
  testCircles(10, ILI9341_WHITE);
  delay_ms(500);
 
  testTriangles();
  delay_ms(500);
 
  testFilledTriangles();
  delay_ms(500);
 
  testRoundRects();
  delay_ms(500);
 
  testFilledRoundRects();
  delay_ms(500);
 
  while(TRUE) {
 
    for(uint8_t rotation=0; rotation<4; rotation++) {
      display_setRotation(rotation);
      testText();
      delay_ms(1000);
    }
 
  }
 
}
 
// end of code.