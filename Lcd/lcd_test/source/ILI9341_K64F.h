/*
 * ILI9341_K64F.h
 *
 *  Created on: 4 ene. 2021
 *      Author: Marcelo
 */

#ifndef ILI9341_K64F_H_
#define ILI9341_K64F_H_

#include <stdio.h>
#include <stdint.h>

/*
 * Definitios
 */
// Color definitions
#define ILI9341_BLACK       0x0000  ///<   0,   0,   0
#define ILI9341_NAVY        0x000F  ///<   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0  ///<   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF  ///<   0, 125, 123
#define ILI9341_MAROON      0x7800  ///< 123,   0,   0
#define ILI9341_PURPLE      0x780F  ///< 123,   0, 123
#define ILI9341_OLIVE       0x7BE0  ///< 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618  ///< 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF  ///< 123, 125, 123
#define ILI9341_BLUE        0x001F  ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0  ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF  ///<   0, 255, 255
#define ILI9341_RED         0xF800  ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F  ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0  ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF  ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20  ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define ILI9341_PINK        0xFC18  ///< 255, 130, 198

/*
 * Public functions
 */

void ili9341_init(void);

void delay_ms_150(void);

void tft_begin(void);

void drawImg(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *img);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void drawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
void drawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void fillScreen(uint16_t color);
void setRotation(uint8_t m);


uint16_t get_display_width(void);
uint16_t get_display_height(void);

void pit_callback(void);

#endif /* ILI9341_K64F_H_ */
