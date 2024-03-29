/*
 * GFX_Lib_K64F.c
 *
 *  Created on: 5 ene. 2021
 *      Author: Marcelo
 */
#include "GFX_Lib_K64F.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
/*
 * Other includes
 */
#include <stdlib.h>
#include "sonic.h"
#include "carp.h"
#include "heart.h"
#include "termo.h"
#include "oxygen.h"
/*
 * Private declarations
 */
void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/*
 * Private defines
 */
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define GRID_COLOR 	0x3186
#define GRID_W 160
#define GRID_H 100
#define GRID_RES 20
/*
 * Internal variables
 */
int16_t
  cursor_x = 0,          ///< x location to start print()ing text
  cursor_y = 0;          ///< y location to start print()ing text
uint16_t
  textcolor = 0xFFFF,    ///< 16-bit background color for print()
  textbgcolor = 0xFFFF;  ///< 16-bit text color for print()
uint8_t
  textsize = 1;          ///< Desired magnification of text to print()
_Bool
  wrap = true;           ///< If set, 'wrap' text at right edge of display
uint16_t				 ///< Grid start references (initialize in lcdGFX_init)
  gridECG_x = 0,
  gridECG_y = 0,
  gridSPO2_x = 0,
  gridSPO2_y = 0;
uint16_t				 ///< Counters for data and grid plot update
  data_cont = 0,
  space_cont = GRID_RES;
uint16_t				 ///< Coordenates for number values
  bpm_x = 0,
  bpm_y = 0,
  temp_x = 0,
  temp_y = 0,
  oxy_x = 0,
  oxy_y = 0;
uint16_t ECG_graph[160];
uint16_t SPO2_graph[160];

// Standard ASCII 5x7 font
#ifndef FONT5X7_H
static const uint8_t font[256][5] = {
   {0x00, 0x00, 0x00, 0x00, 0x00},
   {0x3E, 0x5B, 0x4F, 0x5B, 0x3E},
   {0x3E, 0x6B, 0x4F, 0x6B, 0x3E},
   {0x1C, 0x3E, 0x7C, 0x3E, 0x1C},
   {0x18, 0x3C, 0x7E, 0x3C, 0x18},
   {0x1C, 0x57, 0x7D, 0x57, 0x1C},
   {0x1C, 0x5E, 0x7F, 0x5E, 0x1C},
   {0x00, 0x18, 0x3C, 0x18, 0x00},
   {0xFF, 0xE7, 0xC3, 0xE7, 0xFF},
   {0x00, 0x18, 0x24, 0x18, 0x00},
   {0xFF, 0xE7, 0xDB, 0xE7, 0xFF},
   {0x30, 0x48, 0x3A, 0x06, 0x0E},
   {0x26, 0x29, 0x79, 0x29, 0x26},
   {0x40, 0x7F, 0x05, 0x05, 0x07},
   {0x40, 0x7F, 0x05, 0x25, 0x3F},
   {0x5A, 0x3C, 0xE7, 0x3C, 0x5A},
   {0x7F, 0x3E, 0x1C, 0x1C, 0x08},
   {0x08, 0x1C, 0x1C, 0x3E, 0x7F},
   {0x14, 0x22, 0x7F, 0x22, 0x14},
   {0x5F, 0x5F, 0x00, 0x5F, 0x5F},
   {0x06, 0x09, 0x7F, 0x01, 0x7F},
   {0x00, 0x66, 0x89, 0x95, 0x6A},
   {0x60, 0x60, 0x60, 0x60, 0x60},
   {0x94, 0xA2, 0xFF, 0xA2, 0x94},
   {0x08, 0x04, 0x7E, 0x04, 0x08},
   {0x10, 0x20, 0x7E, 0x20, 0x10},
   {0x08, 0x08, 0x2A, 0x1C, 0x08},
   {0x08, 0x1C, 0x2A, 0x08, 0x08},
   {0x1E, 0x10, 0x10, 0x10, 0x10},
   {0x0C, 0x1E, 0x0C, 0x1E, 0x0C},
   {0x30, 0x38, 0x3E, 0x38, 0x30},
   {0x06, 0x0E, 0x3E, 0x0E, 0x06},
   {0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x5F, 0x00, 0x00},
   {0x00, 0x07, 0x00, 0x07, 0x00},
   {0x14, 0x7F, 0x14, 0x7F, 0x14},
   {0x24, 0x2A, 0x7F, 0x2A, 0x12},
   {0x23, 0x13, 0x08, 0x64, 0x62},
   {0x36, 0x49, 0x56, 0x20, 0x50},
   {0x00, 0x08, 0x07, 0x03, 0x00},
   {0x00, 0x1C, 0x22, 0x41, 0x00},
   {0x00, 0x41, 0x22, 0x1C, 0x00},
   {0x2A, 0x1C, 0x7F, 0x1C, 0x2A},
   {0x08, 0x08, 0x3E, 0x08, 0x08},
   {0x00, 0x80, 0x70, 0x30, 0x00},
   {0x08, 0x08, 0x08, 0x08, 0x08},
   {0x00, 0x00, 0x60, 0x60, 0x00},
   {0x20, 0x10, 0x08, 0x04, 0x02},
   {0x3E, 0x51, 0x49, 0x45, 0x3E},
   {0x00, 0x42, 0x7F, 0x40, 0x00},
   {0x72, 0x49, 0x49, 0x49, 0x46},
   {0x21, 0x41, 0x49, 0x4D, 0x33},
   {0x18, 0x14, 0x12, 0x7F, 0x10},
   {0x27, 0x45, 0x45, 0x45, 0x39},
   {0x3C, 0x4A, 0x49, 0x49, 0x31},
   {0x41, 0x21, 0x11, 0x09, 0x07},
   {0x36, 0x49, 0x49, 0x49, 0x36},
   {0x46, 0x49, 0x49, 0x29, 0x1E},
   {0x00, 0x00, 0x14, 0x00, 0x00},
   {0x00, 0x40, 0x34, 0x00, 0x00},
   {0x00, 0x08, 0x14, 0x22, 0x41},
   {0x14, 0x14, 0x14, 0x14, 0x14},
   {0x00, 0x41, 0x22, 0x14, 0x08},
   {0x02, 0x01, 0x59, 0x09, 0x06},
   {0x3E, 0x41, 0x5D, 0x59, 0x4E},
   {0x7C, 0x12, 0x11, 0x12, 0x7C},
   {0x7F, 0x49, 0x49, 0x49, 0x36},
   {0x3E, 0x41, 0x41, 0x41, 0x22},
   {0x7F, 0x41, 0x41, 0x41, 0x3E},
   {0x7F, 0x49, 0x49, 0x49, 0x41},
   {0x7F, 0x09, 0x09, 0x09, 0x01},
   {0x3E, 0x41, 0x41, 0x51, 0x73},
   {0x7F, 0x08, 0x08, 0x08, 0x7F},
   {0x00, 0x41, 0x7F, 0x41, 0x00},
   {0x20, 0x40, 0x41, 0x3F, 0x01},
   {0x7F, 0x08, 0x14, 0x22, 0x41},
   {0x7F, 0x40, 0x40, 0x40, 0x40},
   {0x7F, 0x02, 0x1C, 0x02, 0x7F},
   {0x7F, 0x04, 0x08, 0x10, 0x7F},
   {0x3E, 0x41, 0x41, 0x41, 0x3E},
   {0x7F, 0x09, 0x09, 0x09, 0x06},
   {0x3E, 0x41, 0x51, 0x21, 0x5E},
   {0x7F, 0x09, 0x19, 0x29, 0x46},
   {0x26, 0x49, 0x49, 0x49, 0x32},
   {0x03, 0x01, 0x7F, 0x01, 0x03},
   {0x3F, 0x40, 0x40, 0x40, 0x3F},
   {0x1F, 0x20, 0x40, 0x20, 0x1F},
   {0x3F, 0x40, 0x38, 0x40, 0x3F},
   {0x63, 0x14, 0x08, 0x14, 0x63},
   {0x03, 0x04, 0x78, 0x04, 0x03},
   {0x61, 0x59, 0x49, 0x4D, 0x43},
   {0x00, 0x7F, 0x41, 0x41, 0x41},
   {0x02, 0x04, 0x08, 0x10, 0x20},
   {0x00, 0x41, 0x41, 0x41, 0x7F},
   {0x04, 0x02, 0x01, 0x02, 0x04},
   {0x40, 0x40, 0x40, 0x40, 0x40},
   {0x00, 0x03, 0x07, 0x08, 0x00},
   {0x20, 0x54, 0x54, 0x78, 0x40},
   {0x7F, 0x28, 0x44, 0x44, 0x38},
   {0x38, 0x44, 0x44, 0x44, 0x28},
   {0x38, 0x44, 0x44, 0x28, 0x7F},
   {0x38, 0x54, 0x54, 0x54, 0x18},
   {0x00, 0x08, 0x7E, 0x09, 0x02},
   {0x18, 0xA4, 0xA4, 0x9C, 0x78},
   {0x7F, 0x08, 0x04, 0x04, 0x78},
   {0x00, 0x44, 0x7D, 0x40, 0x00},
   {0x20, 0x40, 0x40, 0x3D, 0x00},
   {0x7F, 0x10, 0x28, 0x44, 0x00},
   {0x00, 0x41, 0x7F, 0x40, 0x00},
   {0x7C, 0x04, 0x78, 0x04, 0x78},
   {0x7C, 0x08, 0x04, 0x04, 0x78},
   {0x38, 0x44, 0x44, 0x44, 0x38},
   {0xFC, 0x18, 0x24, 0x24, 0x18},
   {0x18, 0x24, 0x24, 0x18, 0xFC},
   {0x7C, 0x08, 0x04, 0x04, 0x08},
   {0x48, 0x54, 0x54, 0x54, 0x24},
   {0x04, 0x04, 0x3F, 0x44, 0x24},
   {0x3C, 0x40, 0x40, 0x20, 0x7C},
   {0x1C, 0x20, 0x40, 0x20, 0x1C},
   {0x3C, 0x40, 0x30, 0x40, 0x3C},
   {0x44, 0x28, 0x10, 0x28, 0x44},
   {0x4C, 0x90, 0x90, 0x90, 0x7C},
   {0x44, 0x64, 0x54, 0x4C, 0x44},
   {0x00, 0x08, 0x36, 0x41, 0x00},
   {0x00, 0x00, 0x77, 0x00, 0x00},
   {0x00, 0x41, 0x36, 0x08, 0x00},
   {0x02, 0x01, 0x02, 0x04, 0x02},
   {0x3C, 0x26, 0x23, 0x26, 0x3C},
   {0x1E, 0xA1, 0xA1, 0x61, 0x12},
   {0x3A, 0x40, 0x40, 0x20, 0x7A},
   {0x38, 0x54, 0x54, 0x55, 0x59},
   {0x21, 0x55, 0x55, 0x79, 0x41},
   {0x22, 0x54, 0x54, 0x78, 0x42}, // a-umlaut
   {0x21, 0x55, 0x54, 0x78, 0x40},
   {0x20, 0x54, 0x55, 0x79, 0x40},
   {0x0C, 0x1E, 0x52, 0x72, 0x12},
   {0x39, 0x55, 0x55, 0x55, 0x59},
   {0x39, 0x54, 0x54, 0x54, 0x59},
   {0x39, 0x55, 0x54, 0x54, 0x58},
   {0x00, 0x00, 0x45, 0x7C, 0x41},
   {0x00, 0x02, 0x45, 0x7D, 0x42},
   {0x00, 0x01, 0x45, 0x7C, 0x40},
   {0x7D, 0x12, 0x11, 0x12, 0x7D}, // A-umlaut
   {0xF0, 0x28, 0x25, 0x28, 0xF0},
   {0x7C, 0x54, 0x55, 0x45, 0x00},
   {0x20, 0x54, 0x54, 0x7C, 0x54},
   {0x7C, 0x0A, 0x09, 0x7F, 0x49},
   {0x32, 0x49, 0x49, 0x49, 0x32},
   {0x3A, 0x44, 0x44, 0x44, 0x3A}, // o-umlaut
   {0x32, 0x4A, 0x48, 0x48, 0x30},
   {0x3A, 0x41, 0x41, 0x21, 0x7A},
   {0x3A, 0x42, 0x40, 0x20, 0x78},
   {0x00, 0x9D, 0xA0, 0xA0, 0x7D},
   {0x3D, 0x42, 0x42, 0x42, 0x3D}, // O-umlaut
   {0x3D, 0x40, 0x40, 0x40, 0x3D},
   {0x3C, 0x24, 0xFF, 0x24, 0x24},
   {0x48, 0x7E, 0x49, 0x43, 0x66},
   {0x2B, 0x2F, 0xFC, 0x2F, 0x2B},
   {0xFF, 0x09, 0x29, 0xF6, 0x20},
   {0xC0, 0x88, 0x7E, 0x09, 0x03},
   {0x20, 0x54, 0x54, 0x79, 0x41},
   {0x00, 0x00, 0x44, 0x7D, 0x41},
   {0x30, 0x48, 0x48, 0x4A, 0x32},
   {0x38, 0x40, 0x40, 0x22, 0x7A},
   {0x00, 0x7A, 0x0A, 0x0A, 0x72},
   {0x7D, 0x0D, 0x19, 0x31, 0x7D},
   {0x26, 0x29, 0x29, 0x2F, 0x28},
   {0x26, 0x29, 0x29, 0x29, 0x26},
   {0x30, 0x48, 0x4D, 0x40, 0x20},
   {0x38, 0x08, 0x08, 0x08, 0x08},
   {0x08, 0x08, 0x08, 0x08, 0x38},
   {0x2F, 0x10, 0xC8, 0xAC, 0xBA},
   {0x2F, 0x10, 0x28, 0x34, 0xFA},
   {0x00, 0x00, 0x7B, 0x00, 0x00},
   {0x08, 0x14, 0x2A, 0x14, 0x22},
   {0x22, 0x14, 0x2A, 0x14, 0x08},
   {0x55, 0x00, 0x55, 0x00, 0x55}, // #176 (25% block) missing in old code
   {0xAA, 0x55, 0xAA, 0x55, 0xAA}, // 50% block
   {0xFF, 0x55, 0xFF, 0x55, 0xFF}, // 75% block
   {0x00, 0x00, 0x00, 0xFF, 0x00},
   {0x10, 0x10, 0x10, 0xFF, 0x00},
   {0x14, 0x14, 0x14, 0xFF, 0x00},
   {0x10, 0x10, 0xFF, 0x00, 0xFF},
   {0x10, 0x10, 0xF0, 0x10, 0xF0},
   {0x14, 0x14, 0x14, 0xFC, 0x00},
   {0x14, 0x14, 0xF7, 0x00, 0xFF},
   {0x00, 0x00, 0xFF, 0x00, 0xFF},
   {0x14, 0x14, 0xF4, 0x04, 0xFC},
   {0x14, 0x14, 0x17, 0x10, 0x1F},
   {0x10, 0x10, 0x1F, 0x10, 0x1F},
   {0x14, 0x14, 0x14, 0x1F, 0x00},
   {0x10, 0x10, 0x10, 0xF0, 0x00},
   {0x00, 0x00, 0x00, 0x1F, 0x10},
   {0x10, 0x10, 0x10, 0x1F, 0x10},
   {0x10, 0x10, 0x10, 0xF0, 0x10},
   {0x00, 0x00, 0x00, 0xFF, 0x10},
   {0x10, 0x10, 0x10, 0x10, 0x10},
   {0x10, 0x10, 0x10, 0xFF, 0x10},
   {0x00, 0x00, 0x00, 0xFF, 0x14},
   {0x00, 0x00, 0xFF, 0x00, 0xFF},
   {0x00, 0x00, 0x1F, 0x10, 0x17},
   {0x00, 0x00, 0xFC, 0x04, 0xF4},
   {0x14, 0x14, 0x17, 0x10, 0x17},
   {0x14, 0x14, 0xF4, 0x04, 0xF4},
   {0x00, 0x00, 0xFF, 0x00, 0xF7},
   {0x14, 0x14, 0x14, 0x14, 0x14},
   {0x14, 0x14, 0xF7, 0x00, 0xF7},
   {0x14, 0x14, 0x14, 0x17, 0x14},
   {0x10, 0x10, 0x1F, 0x10, 0x1F},
   {0x14, 0x14, 0x14, 0xF4, 0x14},
   {0x10, 0x10, 0xF0, 0x10, 0xF0},
   {0x00, 0x00, 0x1F, 0x10, 0x1F},
   {0x00, 0x00, 0x00, 0x1F, 0x14},
   {0x00, 0x00, 0x00, 0xFC, 0x14},
   {0x00, 0x00, 0xF0, 0x10, 0xF0},
   {0x10, 0x10, 0xFF, 0x10, 0xFF},
   {0x14, 0x14, 0x14, 0xFF, 0x14},
   {0x10, 0x10, 0x10, 0x1F, 0x00},
   {0x00, 0x00, 0x00, 0xF0, 0x10},
   {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
   {0xF0, 0xF0, 0xF0, 0xF0, 0xF0},
   {0xFF, 0xFF, 0xFF, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0xFF, 0xFF},
   {0x0F, 0x0F, 0x0F, 0x0F, 0x0F},
   {0x38, 0x44, 0x44, 0x38, 0x44},
   {0xFC, 0x4A, 0x4A, 0x4A, 0x34}, // sharp-s or beta
   {0x7E, 0x02, 0x02, 0x06, 0x06},
   {0x02, 0x7E, 0x02, 0x7E, 0x02},
   {0x63, 0x55, 0x49, 0x41, 0x63},
   {0x38, 0x44, 0x44, 0x3C, 0x04},
   {0x40, 0x7E, 0x20, 0x1E, 0x20},
   {0x06, 0x02, 0x7E, 0x02, 0x02},
   {0x99, 0xA5, 0xE7, 0xA5, 0x99},
   {0x1C, 0x2A, 0x49, 0x2A, 0x1C},
   {0x4C, 0x72, 0x01, 0x72, 0x4C},
   {0x30, 0x4A, 0x4D, 0x4D, 0x30},
   {0x30, 0x48, 0x78, 0x48, 0x30},
   {0xBC, 0x62, 0x5A, 0x46, 0x3D},
   {0x3E, 0x49, 0x49, 0x49, 0x00},
   {0x7E, 0x01, 0x01, 0x01, 0x7E},
   {0x2A, 0x2A, 0x2A, 0x2A, 0x2A},
   {0x44, 0x44, 0x5F, 0x44, 0x44},
   {0x40, 0x51, 0x4A, 0x44, 0x40},
   {0x40, 0x44, 0x4A, 0x51, 0x40},
   {0x00, 0x00, 0xFF, 0x01, 0x03},
   {0xE0, 0x80, 0xFF, 0x00, 0x00},
   {0x08, 0x08, 0x6B, 0x6B, 0x08},
   {0x36, 0x12, 0x36, 0x24, 0x36},
   {0x06, 0x0F, 0x09, 0x0F, 0x06},
   {0x00, 0x00, 0x18, 0x18, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x00},
   {0x30, 0x40, 0xFF, 0x01, 0x01},
   {0x00, 0x1F, 0x01, 0x01, 0x1E},
   {0x00, 0x19, 0x1D, 0x17, 0x12},
   {0x00, 0x3C, 0x3C, 0x3C, 0x3C},
   {0x00, 0x00, 0x00, 0x00, 0x00}  // #255 NBSP
};
#endif // FONT5X7_H

/*
 * Function definitions
 */

void display_setCursor(uint16_t x, uint16_t y)
{
	cursor_x = x;
	cursor_y = y;
}

void display_setTextColor(uint16_t c)
{
	textcolor = c;
	textbgcolor = c;
}

void display_setTextSize(uint8_t s)
{
    textsize = (s > 0) ? s : 1;
}

void display_print(uint8_t c)
{
	if (c == ' ' && cursor_x == 0 && wrap)
		return;
	if(c == '\r')
	{
		cursor_x = 0;
		return;
	}
	if(c == '\n')
	{
		cursor_y += textsize * 8;
		return;
	}
	for(uint8_t i = 0; i < 5; i++ )
	{
		uint8_t line = font[c][i];
		for(uint8_t j = 0; j < 8; j++, line >>= 1)
		{
			if(line & 1)
			{
				if(textsize == 1)
					display_drawPixel(cursor_x + i, cursor_y + j, textcolor);
			    else
			    	display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
		    }
		    else
		    {
		    	if(textbgcolor != textcolor)
		    	{
		    		if(textsize == 1)
		    			display_drawPixel(cursor_x + i, cursor_y + j, textbgcolor);
			        else
			        	display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
			    }
		    }
		}
	}

	if(textbgcolor != textcolor)
	{  // If opaque, draw vertical line for last column
		if(textsize == 1)  display_drawVLine(cursor_x + 5, cursor_y, 8, textbgcolor);
		else               display_fillRect(cursor_x + 5 * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
	}

	cursor_x += textsize * 6;

	if( cursor_x > ((uint16_t)get_display_width() + textsize * 6) )
		cursor_x = get_display_width();

	if (wrap && (cursor_x + (textsize * 5)) > get_display_width())
	{
		cursor_x = 0;
		cursor_y += textsize * 8;
		if( cursor_y > ((uint16_t)get_display_height() + textsize * 8) )
			cursor_y = get_display_height();
	}
}

void display_printString(char *text)
{
	uint8_t i = 0;
	while(text[i] != '\0')
	{
		display_print(text[i]);
		i++;
	}
}

void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    // Update in subclasses if desired!
    if(x0 == x1)
    {
        if(y0 > y1)
        {
        	_swap_int16_t(y0, y1);
        }
        display_drawVLine(x0, y0, y1 - y0 + 1, color);
    }
    else if(y0 == y1)
    {
        if(x0 > x1)
        {
        	_swap_int16_t(x0, x1);
        }
        display_drawHLine(x0, y0, x1 - x0 + 1, color);
    }
    else
    {
        writeLine(x0, y0, x1, y1, color);
    }
}

void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    _Bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
    if (steep)
    {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1)
    {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs((int16_t)(y1 - y0));

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0<=x1; x0++)
    {
        if (steep)
        {
            display_drawPixel(y0, x0, color);
        }
        else
        {
            display_drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    display_drawHLine(x, y, w, color);
    display_drawHLine(x, y+h-1, w, color);
    display_drawVLine(x, y, h, color);
    display_drawVLine(x+w-1, y, h, color);
}

void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    display_drawVLine(x0, y0-r, 2*r+1, color);
    display_fillCircleHelper(x0, y0, r, 3, 0, color);
}

void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corners, uint16_t delta, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) display_drawVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) display_drawVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) display_drawVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) display_drawVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    display_drawPixel(x0  , y0+r, color);
    display_drawPixel(x0  , y0-r, color);
    display_drawPixel(x0+r, y0  , color);
    display_drawPixel(x0-r, y0  , color);

    while (x<y)
    {
        if (f >= 0)
        {
        	y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display_drawPixel(x0 + x, y0 + y, color);
        display_drawPixel(x0 - x, y0 + y, color);
        display_drawPixel(x0 + x, y0 - y, color);
        display_drawPixel(x0 - x, y0 - y, color);
        display_drawPixel(x0 + y, y0 + x, color);
        display_drawPixel(x0 - y, y0 + x, color);
        display_drawPixel(x0 + y, y0 - x, color);
        display_drawPixel(x0 - y, y0 - x, color);
    }
}

void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            display_drawPixel(x0 + x, y0 + y, color);
            display_drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            display_drawPixel(x0 + x, y0 - y, color);
            display_drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            display_drawPixel(x0 - y, y0 + x, color);
            display_drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            display_drawPixel(x0 - y, y0 - x, color);
            display_drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint16_t color)
{
    display_drawLine(x0, y0, x1, y1, color);
    display_drawLine(x1, y1, x2, y2, color);
    display_drawLine(x2, y2, x0, y0, color);
}

void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        display_drawHLine(a, y0, b-a+1, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        display_drawHLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        display_drawHLine(a, y, b-a+1, color);
    }
}

uint16_t display_color565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((uint16_t)(red & 0xF8) << 8) | ((uint16_t)(green & 0xFC) << 3) | (blue >> 3);
}

uint16_t display_getCursorX(void)
{
    return cursor_x;
}

uint16_t display_getCursorY(void)
{
    return cursor_y;
}

void display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint8_t size)
{
	uint16_t prev_x     = cursor_x,
	   prev_y     = cursor_y,
	   prev_color = textcolor,
	   prev_bg    = textbgcolor;
	uint8_t  prev_size  = textsize;

	display_setCursor(x, y);
	display_setTextSize(size);
	display_setTextColor(color);
	display_print(c);

	cursor_x    = prev_x;
	cursor_y    = prev_y;
	textcolor   = prev_color;
	textbgcolor = prev_bg;
	textsize    = prev_size;
}

void display_drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    // smarter version
    display_drawHLine(x+r  , y    , w-2*r, color); // Top
    display_drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    display_drawVLine(x    , y+r  , h-2*r, color); // Left
    display_drawVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    display_drawCircleHelper(x+r    , y+r    , r, 1, color);
    display_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    display_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    display_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

void display_fillRoundRect(uint16_t x, uint16_t y, uint16_t w,
  uint16_t h, uint16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    // smarter version
    display_fillRect(x+r, y, w-2*r, h, color);
    // draw four corners
    display_fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    //display_fillCircleHelper(94, 18, 8, 1, 43, color);
    display_fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

/*
 * Function definitios for final user
 */

void lcdGFX_init(void)
{
	tft_begin();

    display_setRotation(3);
    display_fillScreen(ILI9341_BLACK);
    int16_t w, h;
    w = get_display_width();
    h = get_display_height();

    display_fillScreen(ILI9341_BLACK);
    gridECG_x = 10;
    gridECG_y = 110;
    gridSPO2_x = 10;
    gridSPO2_y = 10+100+(h/2);
    // Border lines
    display_drawHLine(0, 0, w, ILI9341_WHITE);
    display_drawHLine(0, h/2, w, ILI9341_WHITE);
    display_drawHLine(0, h-1, w, ILI9341_WHITE);

    display_drawVLine(0, 0, h, ILI9341_WHITE);
    display_drawVLine(w-1, 0, h, ILI9341_WHITE);

    // Grid for graphics
    // Grid dimensions: 160x100, separated by 20x20 squares
    display_drawHLine(10, 10, 160, GRID_COLOR);
    display_drawHLine(10, 30, 160, GRID_COLOR);
    display_drawHLine(10, 50, 160, GRID_COLOR);
    display_drawHLine(10, 70, 160, GRID_COLOR);
    display_drawHLine(10, 90, 160, GRID_COLOR);
    display_drawHLine(10, 110, 160, GRID_COLOR);
    display_drawVLine(10, 10, 100, GRID_COLOR);
    display_drawVLine(30, 10, 100, GRID_COLOR);
    display_drawVLine(50, 10, 100, GRID_COLOR);
    display_drawVLine(70, 10, 100, GRID_COLOR);
    display_drawVLine(90, 10, 100, GRID_COLOR);
    display_drawVLine(110, 10, 100, GRID_COLOR);
    display_drawVLine(130, 10, 100, GRID_COLOR);
    display_drawVLine(150, 10, 100, GRID_COLOR);
    display_drawVLine(170, 10, 100, GRID_COLOR);

    display_drawHLine(10, 10+(h/2), 160, GRID_COLOR);
    display_drawHLine(10, 30+(h/2), 160, GRID_COLOR);
    display_drawHLine(10, 50+(h/2), 160, GRID_COLOR);
    display_drawHLine(10, 70+(h/2), 160, GRID_COLOR);
    display_drawHLine(10, 90+(h/2), 160, GRID_COLOR);
    display_drawHLine(10, 110+(h/2), 160, GRID_COLOR);
    display_drawVLine(10, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(30, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(50, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(70, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(90, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(110, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(130, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(150, 10+(h/2), 100, GRID_COLOR);
    display_drawVLine(170, 10+(h/2), 100, GRID_COLOR);


    uint8_t numSize = 5;
	bpm_x = (3*w/4) - (numSize*5);
	bpm_y = (h/4) - (numSize*3.5) -20;
    display_setCursor(bpm_x, bpm_y);
	display_setTextColor(ILI9341_GREEN);
	display_setTextSize(numSize);
	display_printString("60\r\n");


    for(uint16_t i=0;i<30;i++)
    {
    	for(uint16_t j=0;j<30;j++)
    	{
    		display_drawPixel(i+280,j+(h/4)-15-20,heart[j][i]);
    	}
    }

    numSize = 1;
    display_setCursor(280+7.5, 10);
	display_setTextColor(ILI9341_GREEN);
	display_setTextSize(numSize);
	display_printString("bpm\r\n");

    numSize = 1;
    display_setCursor(280-65, 10);
	display_setTextColor(ILI9341_GREEN);
	display_setTextSize(numSize);
	display_printString("HR\r\n");

	int16_t tempRef = 10;
    numSize = 1;
    display_setCursor(280-65, (h/4)+tempRef);
	display_setTextColor(ILI9341_YELLOW);
	display_setTextSize(numSize);
	display_printString("Temperature\r\n");

    for(uint16_t i=0;i<15;i++)
    {
    	for(uint16_t j=0;j<33;j++)
    	{
    		display_drawPixel(i+297,j+(h/4)+10,termo[j][i]);
    	}
    }

    numSize = 2;
    temp_x = 280-65;
    temp_y = display_getCursorY()+10;
    display_setCursor(temp_x, temp_y);
	display_setTextColor(ILI9341_YELLOW);
	display_setTextSize(numSize);
	display_printString("36.5");
	int16_t txtRef_x, txtRef_y;
	txtRef_x = display_getCursorX();
	txtRef_y = display_getCursorY();

	display_drawCircle(display_getCursorX()+10, display_getCursorY()+2, 2, ILI9341_YELLOW);

    display_setCursor(txtRef_x +15, txtRef_y);
	display_setTextColor(ILI9341_YELLOW);
	display_setTextSize(numSize);
	display_printString("C");

	int16_t rect_w,rect_h;
	rect_w = 16;
	rect_h = (h/2)-2;
	display_fillRect(280-65-(2*rect_w), 1, rect_w, rect_h, ILI9341_BLUE);
	display_fillRect(280-65-(2*rect_w), (h/2)+1, rect_w, rect_h, ILI9341_BLUE);
	display_drawVLine(280-65-(2*rect_w), 0, 240, ILI9341_WHITE);
	display_drawVLine(280-65-(rect_w), 0, 240, ILI9341_WHITE);

    numSize = 2;
    display_setCursor(280-65-(2*rect_w)+3.5, 30);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(numSize);
	display_printString("E\n");
	display_setCursor(280-65-(2*rect_w)+3.5, display_getCursorY()+7);
	display_printString("C\n");
	display_setCursor(280-65-(2*rect_w)+3.5, display_getCursorY()+7);
	display_printString("G\n");

    numSize = 6;
    oxy_x = 280-65;
    oxy_y = (3*h/4) - (numSize*3.5)+10;
    display_setCursor(oxy_x, oxy_y);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(numSize);
	display_printString("98");
	txtRef_x = display_getCursorX();
	txtRef_y = display_getCursorY();
	display_drawChar(txtRef_x, txtRef_y+27, '%', ILI9341_WHITE, 2);
	numSize = 2;
	display_setCursor(txtRef_x, txtRef_y);
	display_setTextSize(numSize);
	display_printString("89");

    numSize = 1;
    display_setCursor(280-65, (h/2)+20);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(numSize);
	display_printString("PR\r\n");

    for(uint16_t i=0;i<30;i++)
    {
    	for(uint16_t j=0;j<30;j++)
    	{
    		display_drawPixel(i+(3*w/4)+5,j+(h/2)+7,oxygen[j][i]);
    	}
    }

    display_setCursor(280+7.5, (h/2)+20);
	display_printString("bpm\r\n");

    numSize = 2;
    display_setCursor(280-65-(2*rect_w)+3.5, (h/2) +20);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(numSize);
	display_printString("S\n");
	display_setCursor(280-65-(2*rect_w)+3.5, display_getCursorY()+7);
	display_printString("P\n");
	display_setCursor(280-65-(2*rect_w)+3.5, display_getCursorY()+7);
	display_printString("O\n");
	display_setCursor(280-65-(2*rect_w)+3.5, display_getCursorY()+7);
	display_printString("2\n");

	PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1);
}

void lcdGFX_updateGFX(uint16_t dataECG, uint16_t dataSPO2)
{
	uint16_t h = get_display_height();

    if((!(ECG_graph[space_cont]%GRID_RES))||(!(space_cont%GRID_RES))) // Limit of squares
    {
    	display_drawPixel(10+space_cont, 10+100-ECG_graph[space_cont], GRID_COLOR);
//    	display_drawVLine(10+space_cont, 10, GRID_H, GRID_COLOR);
//    	display_drawVLine(10+space_cont, 10+(h/2), GRID_H, GRID_COLOR);
    }
    else
    {
    	display_drawPixel(10+space_cont, 10+100-ECG_graph[space_cont], ILI9341_BLACK);
//    	display_drawVLine(10+space_cont, 10, GRID_H, ILI9341_BLACK);
//        display_drawPixel(10+space_cont, 10, GRID_COLOR);
//        display_drawPixel(10+space_cont, 30, GRID_COLOR);
//        display_drawPixel(10+space_cont, 50, GRID_COLOR);
//        display_drawPixel(10+space_cont, 70, GRID_COLOR);
//        display_drawPixel(10+space_cont, 90, GRID_COLOR);
//        display_drawPixel(10+space_cont, 110, GRID_COLOR);
//
//    	display_drawVLine(10+space_cont, 10+(h/2), GRID_H, ILI9341_BLACK);
//        display_drawPixel(10+space_cont, 10+(h/2), GRID_COLOR);
//        display_drawPixel(10+space_cont, 30+(h/2), GRID_COLOR);
//        display_drawPixel(10+space_cont, 50+(h/2), GRID_COLOR);
//        display_drawPixel(10+space_cont, 70+(h/2), GRID_COLOR);
//        display_drawPixel(10+space_cont, 90+(h/2), GRID_COLOR);
//        display_drawPixel(10+space_cont, 110+(h/2), GRID_COLOR);
    }
    if((!(SPO2_graph[space_cont]%GRID_RES))||(!(space_cont%GRID_RES))) // Limit of squares
    {
    	display_drawPixel(10+space_cont, (h/2)+10+100-SPO2_graph[space_cont], GRID_COLOR);
//    	display_drawVLine(10+space_cont, 10, GRID_H, GRID_COLOR);
//    	display_drawVLine(10+space_cont, 10+(h/2), GRID_H, GRID_COLOR);
    }
    else
    {
    	display_drawPixel(10+space_cont, (h/2)+10+100-SPO2_graph[space_cont], ILI9341_BLACK);
    }
    // Drawing graphics
    display_drawPixel(gridECG_x+data_cont, gridECG_y-dataECG, ILI9341_GREEN);
    ECG_graph[data_cont] = dataECG;
    display_drawPixel(gridSPO2_x+data_cont, gridSPO2_y-dataSPO2, ILI9341_WHITE);
    SPO2_graph[data_cont] = dataSPO2;

    if(space_cont == GRID_W)
    	space_cont = 0;
    else
    	space_cont++;

    if(data_cont == GRID_W)
    	data_cont = 0;
    else
    	data_cont++;
}

void lcdGFX_updateDATA(uint16_t dataECG, float dataSPO2, float dataTEMP)
{
    // Transcode numbers and print
    uint8_t numSize;
    char dec, uni, dec1, dec2;

	// Update ECG Value
	numSize = 5;
	display_setCursor(bpm_x, bpm_y);
	display_setTextColor(ILI9341_GREEN);
	display_setTextSize(numSize);
	dec = (dataECG/10) + '0';
	uni = (dataECG%10) + '0';
	display_fillRect(bpm_x, bpm_y, 60, 35, ILI9341_BLACK); // Erase previous value
	display_print(dec);
	display_print(uni);

	// Update Temperature Values
	numSize = 2;
	display_fillRect(temp_x, temp_y, 55, 15, ILI9341_BLACK); // Erase previous value
	display_setCursor(temp_x, temp_y);
	display_setTextColor(ILI9341_YELLOW);
	display_setTextSize(numSize);
	dec = dataTEMP/10 + '0';
	uni = ((uint16_t)(dataTEMP))%10 + '0';
	dec1 = ((uint16_t)(dataTEMP*10))%10 + '0';
	display_print(dec);
	display_print(uni);
	display_print('.');
	display_print(dec1);

	// Update SPO2 Value
	numSize = 6; // Text size 3.5x5
	display_fillRect(oxy_x, oxy_y, 67+30, 50 , ILI9341_BLACK); // Erase previous value
//	display_fillRect(oxy_x+67, oxy_y, 30, 15, ILI9341_BLACK); // Erase previous value
	display_setCursor(oxy_x, oxy_y);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(numSize);
	dec = dataSPO2/10 + '0';
	uni = ((uint16_t)(dataSPO2))%10 + '0';
	dec1 = ((uint16_t)(dataSPO2*10))%10 + '0';
	dec2 = ((uint16_t)(dataSPO2*100))%10 + '0';
	display_print(dec);
	display_print(uni);
	int16_t txtRef_x, txtRef_y;
	txtRef_x = display_getCursorX();
	txtRef_y = display_getCursorY();
	numSize = 2;
	display_setTextSize(numSize);
	display_drawChar(txtRef_x, txtRef_y+27, '%', ILI9341_WHITE, 2);
	display_setCursor(txtRef_x, txtRef_y);
	display_print(dec1);
	display_print(dec2);

}
