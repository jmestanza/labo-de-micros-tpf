/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    lcd_test.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "GFX_Lib_K64F.h"
/* TODO: insert other definitions and declarations here. */

/*
 * Test functions declarations
 */
void testFillScreen(void);
void testText(void);
void testLines(uint16_t color);
void testFastLines(uint16_t color1, uint16_t color2);
void testRects(uint16_t color);
void testFilledRects(uint16_t color1, uint16_t color2);
void testFilledCircles(uint8_t radius, uint16_t color);
void testCircles(uint8_t radius, uint16_t color);
void testTriangles(void);
void testFilledTriangles(void);
void testRoundRects(void);
void testFilledRoundRects(void);

uint16_t i,j;
uint16_t bpm[160];
uint16_t oxy[160];
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

    /*
     * Test code for monitor
     */
    lcdGFX_init();

    // Test graphics vectors (generated manually)

    for(uint16_t i=0;i<160;i++)
    {
    	oxy[i] = 20*sin(2*PI*i/30)+40;
    }

    for(uint16_t i=0;i<160;i++)
    {
    	bpm[i] = 40;
    }
    for(uint16_t i=0;i<10;i++)
    {
    	bpm[i+40] = 40+(i*40/10);
    	bpm[i+40+80] = 40+(i*40/10);
    }
    for(uint16_t i=0;i<15;i++)
    {
    	bpm[i+50] = 80-(i*60/15);
    	bpm[i+50+80] = 80-(i*60/15);
    }
    for(uint16_t i=0;i<5;i++)
    {
    	bpm[i+65] = 20+(i*60/15);
    	bpm[i+65+80] = 20+(i*60/15);
    }

	/*
	 * End code for monitor
	 * Then update periodic as efficiently as it can be made
	 */

    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    i = 0;
    j = 0;
    while(1) {

    }
    return 0 ;
}

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pit_callback();
}

void PIT1_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
	i++;
	j++;
	lcdGFX_updateGFX(bpm[i%160], oxy[i%160]);
	if(!(j%20))
	{
 		lcdGFX_updateDATA(i%99, 82.34, 36.8);
	}
}
/*
 * Test functions
 */

void testFillScreen(void)
{
	display_fillScreen(ILI9341_BLACK);
	display_fillScreen(ILI9341_RED);
	display_fillScreen(ILI9341_GREEN);
	display_fillScreen(ILI9341_BLUE);
	display_fillScreen(ILI9341_BLACK);
}

void testText(void)
{
	display_fillScreen(ILI9341_BLACK);
	display_setCursor(0, 0);
	display_setTextColor(ILI9341_WHITE);
	display_setTextSize(1);
	display_printString("Hello World!\r\n");
	display_setTextColor(ILI9341_YELLOW);
	display_setTextSize(2);
	display_printString("1234.56\r\n");
	display_setTextColor(ILI9341_RED);
	display_setTextSize(3);
	display_printString("0xDEADBEEF\r\n");
	display_printString("\r\n");    // start a new line
	display_setTextColor(ILI9341_GREEN);
	display_setTextSize(5);
	display_printString("Groop\r\n");
	display_setTextSize(2);
	display_printString("I implore thee,\r\n");
	display_setTextSize(1);
	display_printString("my foonting turlingdromes.\r\n");
	display_printString("And hooptiously drangle me\r\n");
	display_printString("with crinkly bindlewurdles,\r\n");
	display_printString("Or I will rend thee\r\n");
	display_printString("in the gobberwarts\r\n");
	display_printString("with my blurglecruncheon,\r\n");
	display_printString("see if I don't!\r\n");
}

void testLines(uint16_t color) {
	int16_t x1, y1, x2, y2,
	w = get_display_width(),
	h = get_display_height();

	display_fillScreen(ILI9341_BLACK);

	x1 = 0; y1 = 0;
	y2 = h - 1;

	for(x2=0; x2<w; x2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	x2 = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	display_fillScreen(ILI9341_BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;

	for(x2=0; x2<w; x2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	x2    = 0;
	for(y2=0; y2<h; y2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	display_fillScreen(ILI9341_BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;

	for(x2=0; x2<w; x2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	x2    = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	display_fillScreen(ILI9341_BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;

	for(x2=0; x2<w; x2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for(y2=0; y2<h; y2+=6)
	{
		display_drawLine(x1, y1, x2, y2, color);
	}
}

void testFastLines(uint16_t color1, uint16_t color2) {
	int16_t x, y, w = get_display_width(), h = get_display_height();

	display_fillScreen(ILI9341_BLACK);

	for(y=0; y<h; y+=5)
	{
		display_drawHLine(0, y, w, color1);
	}
	for(x=0; x<w; x+=5)
	{
		display_drawVLine(x, 0, h, color2);
	}
}

void testRects(uint16_t color)
{
	int16_t n, i, i2,
		  cx = get_display_width()  / 2,
		  cy = get_display_height() / 2;

	display_fillScreen(ILI9341_BLACK);
	n = min(get_display_width(), get_display_height());

	for(i=2; i<n; i+=6)
	{
		i2 = i / 2;
		display_drawRect(cx-i2, cy-i2, i, i, color);
	}
}

void testFilledRects(uint16_t color1, uint16_t color2)
{
  int16_t n, i, i2,
          cx = get_display_width()  / 2 - 1,
          cy = get_display_height() / 2 - 1;

  display_fillScreen(ILI9341_BLACK);
  n = min(get_display_width(), get_display_height());
  for(i=n; i>0; i-=6)
  {
	  i2 = i / 2 - 1;
      display_fillRect(cx-i2, cy-i2, i, i, color1);

      // Outlines are not included in timing results
      display_drawRect(cx-i2, cy-i2, i, i, color2);
  }
}

void testFilledCircles(uint8_t radius, uint16_t color)
{
	int16_t x, y, w = get_display_width(), h = get_display_height(), r2 = radius * 2;

	display_fillScreen(ILI9341_BLACK);

	for(x=radius; x<w; x+=r2)
	{
		for(y=radius; y<h; y+=r2)
		{
			display_fillCircle(x, y, radius, color);
	    }
	}
}

void testCircles(uint8_t radius, uint16_t color)
{
	int16_t x, y, r2 = radius * 2,
		  w = get_display_width()  + radius,
		  h = get_display_height() + radius;

	for(x=0; x<w; x+=r2)
	{
		for(y=0; y<h; y+=r2)
		{
			display_drawCircle(x, y, radius, color);
		}
	}
}

void testTriangles(void)
{
  int16_t n, i, cx = get_display_width()  / 2 - 1,
          cy = get_display_height() / 2 - 1;
  uint16_t color = 0xF600;
  display_fillScreen(ILI9341_BLACK);
  n = min(cx, cy);

  for(i=0; i<n; i+=5)
  {
    display_drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      color);
    color += 100;
  }
}

void testFilledTriangles(void)
{
	int16_t i, cx = get_display_width()  / 2 - 1,
			 cy = get_display_height() / 2 - 1;

	display_fillScreen(ILI9341_BLACK);

	for(i=min(cx,cy); i>10; i-=5) {

	display_fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
	  display_color565(0, i*2, i*2));

	display_drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
	  display_color565(i*2, i*2, 0));

	}
}

void testRoundRects(void)
{
	int16_t w, i, i2,
		  cx = get_display_width()  / 2 - 1,
		  cy = get_display_height() / 2 - 1;

	display_fillScreen(ILI9341_BLACK);
	w = min(get_display_width(), get_display_height());

	for(i=0; i<w; i+=6) {
	i2 = i / 2;
	display_drawRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(i, 0, 0));
	}
}

void testFilledRoundRects(void)
{
	int16_t i, i2,
		  cx = get_display_width()  / 2 - 1,
		  cy = get_display_height() / 2 - 1;

	display_fillScreen(ILI9341_BLACK);

	for(i=min(get_display_width(), get_display_height()); i>20; i-=6) {
	i2 = i / 2;
	display_fillRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(0, i, 0));
	}
}
