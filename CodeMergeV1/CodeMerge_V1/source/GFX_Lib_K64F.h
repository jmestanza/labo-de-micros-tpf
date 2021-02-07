/*
 * GFX_Lib_K64F.h
 *
 *  Created on: 5 ene. 2021
 *      Author: Marcelo
 */

#ifndef GFX_LIB_K64F_H_
#define GFX_LIB_K64F_H_

#include <stdbool.h>
#include <stdint.h>
#include "math.h"
#include "ILI9341_K64F.h"
/*
 * Defines for abreviation
 */
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define PI 3.1416

#define display_drawPixel   drawPixel
#define display_drawVLine   drawVLine
#define display_drawHLine   drawHLine
#define display_fillScreen  fillScreen
#define display_fillRect    fillRect
#define display_setRotation setRotation

/*
 * Function declarations GFX
 */
void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color);
void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint16_t color);
void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
void display_fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);

void display_setCursor(uint16_t x, uint16_t y);
void display_setTextColor(uint16_t c);
void display_setTextSize(uint8_t s);
void display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint8_t size);
void display_printString(char *text);

uint16_t display_getCursorX(void);
uint16_t display_getCursorY(void);
uint16_t display_color565(uint8_t red, uint8_t green, uint8_t blue);

/*
 * Functions for final user
 */
void lcdGFX_init(void);
_Bool lcdGFX_getState(void);
void lcdGFX_updateGFX(uint16_t dataECG, uint16_t dataSPO2);
void lcdGFX_updateDATA(uint16_t dataECG, float dataSPO2, float dataTEMP);

#endif /* GFX_LIB_K64F_H_ */
