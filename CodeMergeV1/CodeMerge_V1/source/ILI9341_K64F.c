/*
 * ILI9341_K64F.c
 *
 *  Created on: 4 ene. 2021
 *      Author: Marcelo
 */

#include "ILI9341_K64F.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_dspi.h"
/*
 * Other defines
 */
#define RST_HIGH() GPIO_PortSet(BOARD_ILI9341_RST_GPIO, 1U << BOARD_ILI9341_RST_PIN)
#define RST_LOW() GPIO_PortClear(BOARD_ILI9341_RST_GPIO, 1U << BOARD_ILI9341_RST_PIN)
#define DC_HIGH() GPIO_PortSet(BOARD_ILI9341_DC_GPIO, 1U << BOARD_ILI9341_DC_PIN)
#define DC_LOW() GPIO_PortClear(BOARD_ILI9341_DC_GPIO, 1U << BOARD_ILI9341_DC_PIN)


/*
 * ILI9341 Register defines
 */
#define ILI9341_TFTWIDTH   240      ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT  320      ///< ILI9341 max TFT height

#define ILI9341_NOP        0x00     ///< No-op register
#define ILI9341_SWRESET    0x01     ///< Software reset register
#define ILI9341_RDDID      0x04     ///< Read display identification information
#define ILI9341_RDDST      0x09     ///< Read Display Status

#define ILI9341_SLPIN      0x10     ///< Enter Sleep Mode
#define ILI9341_SLPOUT     0x11     ///< Sleep Out
#define ILI9341_PTLON      0x12     ///< Partial Mode ON
#define ILI9341_NORON      0x13     ///< Normal Display Mode ON

#define ILI9341_RDMODE     0x0A     ///< Read Display Power Mode
#define ILI9341_RDMADCTL   0x0B     ///< Read Display MADCTL
#define ILI9341_RDPIXFMT   0x0C     ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT   0x0D     ///< Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F     ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF     0x20     ///< Display Inversion OFF
#define ILI9341_INVON      0x21     ///< Display Inversion ON
#define ILI9341_GAMMASET   0x26     ///< Gamma Set
#define ILI9341_DISPOFF    0x28     ///< Display OFF
#define ILI9341_DISPON     0x29     ///< Display ON

#define ILI9341_CASET      0x2A     ///< Column Address Set
#define ILI9341_PASET      0x2B     ///< Page Address Set
#define ILI9341_RAMWR      0x2C     ///< Memory Write
#define ILI9341_RAMRD      0x2E     ///< Memory Read

#define ILI9341_PTLAR      0x30     ///< Partial Area
#define ILI9341_MADCTL     0x36     ///< Memory Access Control
#define ILI9341_VSCRSADD   0x37     ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT     0x3A     ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1    0xB1     ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2    0xB2     ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3    0xB3     ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR     0xB4     ///< Display Inversion Control
#define ILI9341_DFUNCTR    0xB6     ///< Display Function Control

#define ILI9341_PWCTR1     0xC0     ///< Power Control 1
#define ILI9341_PWCTR2     0xC1     ///< Power Control 2
#define ILI9341_PWCTR3     0xC2     ///< Power Control 3
#define ILI9341_PWCTR4     0xC3     ///< Power Control 4
#define ILI9341_PWCTR5     0xC4     ///< Power Control 5
#define ILI9341_VMCTR1     0xC5     ///< VCOM Control 1
#define ILI9341_VMCTR2     0xC7     ///< VCOM Control 2

#define ILI9341_RDID1      0xDA     ///< Read ID 1
#define ILI9341_RDID2      0xDB     ///< Read ID 2
#define ILI9341_RDID3      0xDC     ///< Read ID 3
#define ILI9341_RDID4      0xDD     ///< Read ID 4

#define ILI9341_GMCTRP1    0xE0     ///< Positive Gamma Correction
#define ILI9341_GMCTRN1    0xE1     ///< Negative Gamma Correction
//#define ILI9341_PWCTR6     0xFC

#define MADCTL_MY  0x80  ///< Bottom to top
#define MADCTL_MX  0x40  ///< Right to left
#define MADCTL_MV  0x20  ///< Reverse Mode
#define MADCTL_ML  0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00  ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08  ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04  ///< LCD refresh right to left

/*
 * Variables
 */
uint16_t
  _width,    ///< Display width as modified by current rotation
  _height;   ///< Display height as modified by current rotation
uint8_t
  rotation;  ///< Display rotation (0 thru 3)

uint8_t initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};

_Bool pitIsrFlag = false;
dspi_transfer_t masterXfer;
uint8_t masterBuffer[] = {0};
uint8_t buffer[2*ILI9341_TFTWIDTH*ILI9341_TFTHEIGHT/4];

/*
 * Function definitios
 */

void ili9341_init(void)
{
	/*
	 * do something
	 */
	DC_HIGH();
}

void pit_callback(void)
{
	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
	pitIsrFlag = true;
}

uint16_t get_display_width(void)
{
	return _width;
}

uint16_t get_display_height(void)
{
	return _height;
}

void delay_ms_150(void)
{
	PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
	while(!pitIsrFlag);
	pitIsrFlag = false;
}

void writeCommand(uint8_t cmd)
{
	  DC_LOW();
	  /* Start master transfer, send data to slave */
	  masterBuffer[0] = cmd;
	  masterXfer.txData = masterBuffer;
	  masterXfer.rxData = NULL;
	  masterXfer.dataSize = 1U;
	  masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	  DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);
	  /* Then release DC pin */
	  DC_HIGH();
}

void writeData(uint8_t data)
{
	  masterBuffer[0] = data;
	  masterXfer.txData = masterBuffer;
	  masterXfer.rxData = NULL;
	  masterXfer.dataSize = 1U;
	  masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	  DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);
}

void tft_begin(void)
{
//	#ifdef TFT_RST
	  RST_HIGH();
	  delay_ms_150();
	  RST_LOW();
	  delay_ms_150();
	  RST_HIGH();
	  delay_ms_150();
//	#else                           // If no hardware reset pin...
//	  writeCommand(ILI9341_SWRESET);  // Engage software reset
//	  delay_ms(150);
//	#endif

	uint8_t cmd, x, numArgs;
	uint8_t *addr = initcmd;
	cmd = *addr++;
	while( cmd > 0 )
	{
		writeCommand(cmd);
		x = *addr++;
		numArgs = x & 0x7F;
		while(numArgs--)
		{
			uint8_t aux_data;
			aux_data = *addr++;
			writeData(aux_data);
		}
		if(x & 0x80)
		{
			delay_ms_150();
		}
		cmd = *addr++;
	}

	_width  = ILI9341_TFTWIDTH;
	_height = ILI9341_TFTHEIGHT;
	rotation = 0;
}

void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
    uint16_t x2 = (x1 + w - 1),
             y2 = (y1 + h - 1);
    writeCommand(ILI9341_CASET); // Column address set
    buffer[0] = x1 >> 8;
    buffer[1] = x1;
    buffer[2] = x2 >> 8;
    buffer[3] = x2;
	masterXfer.txData = buffer;
	masterXfer.rxData = NULL;
	masterXfer.dataSize = 4;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

//    writeData(x1 >> 8);
//    writeData(x1);
//    writeData(x2 >> 8);
//    writeData(x2);

    writeCommand(ILI9341_PASET); // Row address set
    buffer[0] = y1 >> 8;
    buffer[1] = y1;
    buffer[2] = y2 >> 8;
    buffer[3] = y2;
	masterXfer.txData = buffer;
	masterXfer.rxData = NULL;
	masterXfer.dataSize = 4;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);
//    writeData(y1 >> 8);
//    writeData(y1);
//    writeData(y2 >> 8);
//    writeData(y2);

    writeCommand(ILI9341_RAMWR); // Write to RAM
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	if(w && h) {                            // Nonzero width and height?
		uint8_t hi = color >> 8, lo = color;
		if((x >= _width) || (y >= _height))
		  return;
		if((x + w - 1) >= _width)
		  w = _width  - x;
		if((y + h - 1) >= _height)
		  h = _height - y;
		setAddrWindow(x, y, w, h);
		uint32_t px = (uint32_t)w * h;

		for(uint32_t i=0;i<((2*px)-1);i++)
		{
			if(!(i%2))
			{
				buffer[i] = hi;
			}
			else
			{
				buffer[i] = lo;
			}
		}
		masterXfer.txData = buffer;
		masterXfer.rxData = NULL;
		masterXfer.dataSize = (2*px);
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

//		while (px--) {
//		  writeData(hi);
//		  writeData(lo);
//		}
	}
}

void fillScreen(uint16_t color)
{
    fillRect(0, 0, _width, _height/4, color);
    fillRect(0, _height/4, _width, _height/4, color);
    fillRect(0, _height/2, _width, _height/4, color);
    fillRect(0, 3*_height/4, _width, _height/4, color);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if((x < _width) && (y < _height))
	{
		setAddrWindow(x, y, 1, 1);
		buffer[0] = color >> 8;
		buffer[1] = color & 0xFF;
		masterXfer.txData = buffer;
		masterXfer.rxData = NULL;
		masterXfer.dataSize = 2;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

//		writeData(color >> 8);
//		writeData(color & 0xFF);
	}
}

void drawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color)
{
	if( (x < _width) && (y < _height) && h)
	{
		uint8_t hi = color >> 8, lo = color;
		if((y + h - 1) >= _height)
			h = _height - y;
		setAddrWindow(x, y, 1, h);

		for(uint32_t i=0;i<((2*h)-1);i++)
		{
			if(!(i%2))
			{
				buffer[i] = hi;
			}
			else
			{
				buffer[i] = lo;
			}
		}
		masterXfer.txData = buffer;
		masterXfer.rxData = NULL;
		masterXfer.dataSize = 2*h;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

//		while (h--)
//		{
//			writeData(hi);
//			writeData(lo);
//		}
	}
}

void drawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
	if( (x < _width) && (y < _height) && w)
	{
		uint8_t hi = color >> 8, lo = color;
		if((x + w - 1) >= _width)
			w = _width  - x;
		setAddrWindow(x, y, w, 1);

		for(uint32_t i=0;i<((2*w)-1);i++)
		{
			if(!(i%2))
			{
				buffer[i] = hi;
			}
			else
			{
				buffer[i] = lo;
			}
		}
		masterXfer.txData = buffer;
		masterXfer.rxData = NULL;
		masterXfer.dataSize = 2*w;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

//		while (w--)
//		{
//		  writeData(hi);
//		  writeData(lo);
//		}

	}
}

void setRotation(uint8_t m)
{
  rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            m = (MADCTL_MX | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 1:
            m = (MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
        case 2:
            m = (MADCTL_MY | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 3:
            m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
    }

    writeCommand(ILI9341_MADCTL);
    writeData(m);
}

void drawImg(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *img)
{
	if(w && h) {                            // Nonzero width and height?
		uint8_t hi, lo;
		if((x >= _width) || (y >= _height))
		  return;
		if((x + w - 1) >= _width)
		  w = _width  - x;
		if((y + h - 1) >= _height)
		  h = _height - y;
		setAddrWindow(x, y, w, h);

		uint32_t px = (uint32_t)w * h;
		uint32_t j=0;
		for(uint32_t i=0;i<(px-1);i++)
		{
			hi = img[i] >> 8;
			lo = img[i];
			buffer[j] = hi;
			buffer[j+1] = lo;
			j+=2;
		}

		masterXfer.txData = buffer;
		masterXfer.rxData = NULL;
		masterXfer.dataSize = (2*px);
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI_0_PERIPHERAL, &masterXfer);

	}
}


