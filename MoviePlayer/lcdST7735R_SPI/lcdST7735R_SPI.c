/*
 * lcdST7735R_SPI.c
 *
 * Created: 2016-03-03 11:14:38 PM
 *  Author: take-iwiw
 *        : initial sequence is retrieved from some samples
 *   Note : cannot use spiSendFast(buffer mode) because of CS, RS control
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "lcdST7735R_SPIConfig.h"
#include "lcdST7735R_SPI.h"
#include "lcdST7735R_CMD.h"
#include "../spi/spi.h"

/*** Internal Const Values, Macros ***/
#define DISABLE_LCD_CS (SET_BIT(LCD_ST7735R_SPI_CS_PORT, LCD_ST7735R_SPI_CS_BIT))
#define ENABLE_LCD_CS  (CLR_BIT(LCD_ST7735R_SPI_CS_PORT, LCD_ST7735R_SPI_CS_BIT))
#define SET_LCD_DATA   (SET_BIT(LCD_ST7735R_SPI_DC_PORT, LCD_ST7735R_SPI_DC_BIT))
#define SET_LCD_CMD    (CLR_BIT(LCD_ST7735R_SPI_DC_PORT, LCD_ST7735R_SPI_DC_BIT))

/*** Internal Static Variables ***/
static uint8_t s_isInitialized = 0;

/*** Internal Function Declarations ***/
static void intLcd();
static void sendCmd(uint8_t cmd);
static void sendData(uint8_t data);

/*** External Function Defines ***/
void lcdST7735R_init()
{
	if(s_isInitialized != 0) return;
	s_isInitialized = 1;
	// PORT DDR
	SET_BIT(LCD_ST7735R_SPI_DC_DDR, LCD_ST7735R_SPI_DC_BIT);	// DC
	SET_BIT(LCD_ST7735R_SPI_CS_DDR, LCD_ST7735R_SPI_CS_BIT);	// CS
	// init SPI
	SPI_OPEN_PRM prm;
	prm.order = SPI_OPEN_ORDER_MSB_FIRST;
	prm.role = SPI_OPEN_ROLE_MASTER;
	prm.mode = SPI_OPEN_MODE_0;
	prm.div = SPI_OPEN_DIV_4;
	prm.speed = SPI_OPEN_SPEED_X2;
	prm.blocking = SPI_OPEN_BLOCKING_YES;
	spiOpen(&prm);
	intLcd();
	lcdST7735R_fillRect(0x0000, 0, 0, LCD_ST7735R_WIDTH, LCD_ST7735R_HEIGHT);
}


void lcdST7735R_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	sendCmd(ST7735_CASET);  // column addr set
	sendData(0x00);
	sendData(x0+0);   // XSTART
	sendData(0x00);
	sendData(x1-1);   // XEND

	sendCmd(ST7735_RASET);  // row addr set
	sendData(0x00);
	sendData(y0+0);    // YSTART
	sendData(0x00);
	sendData(y1-1);    // YEND
}

void lcdST7735R_setAddrWindowFull()
{
	lcdST7735R_setAddrWindow(0, 0, LCD_ST7735R_WIDTH, LCD_ST7735R_HEIGHT);
}

void lcdST7735R_fillRect(uint16_t color, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t width = x1 - x0;
	uint8_t height = y1 - y0;

	lcdST7735R_setAddrWindow(x0, y0, x1, y1);
	sendCmd(ST7735_RAMWR);  // write to RAM

	SET_LCD_DATA;
	ENABLE_LCD_CS;
	for (uint8_t x=0; x < width; x++) {
		for (uint8_t y=0; y < height; y++) {
			spiSendBlocking(color >> 8);
			spiSendBlocking(color);
		}
	}
	DISABLE_LCD_CS;
	lcdST7735R_setAddrWindowFull();
}


void lcdST7735R_startSendBurstData()
{
	sendCmd(ST7735_RAMWR);  // write to RAM
	SET_LCD_DATA;
	ENABLE_LCD_CS;
}


inline void lcdST7735R_sendBurstData(uint8_t dataHigh, uint8_t dataLow)
{
	spiSendBlocking(dataHigh);
	spiSendBlocking(dataLow);
}


void lcdST7735R_stopSendBurstData()
{
	DISABLE_LCD_CS;
}

void lcdST7735R_pauseSendBurstData()
{
	DISABLE_LCD_CS;
}

void lcdST7735R_restartSendBurstData()
{
	ENABLE_LCD_CS;
}

void lcdST7735R_setRasterVertical()
{
	sendCmd(ST7735_MADCTL);  // memory access control (directions)
	uint8_t madctl;
	switch(MOUNT_ROTATE){
	case LCD_ST7735R_ROTATE_0:
		madctl = (0<<7) | (0<<6) | (1<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_90:
		madctl = (1<<7) | (0<<6) | (0<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_270:
		madctl = (0<<7) | (1<<6) | (0<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_180:
		madctl = (1<<7) | (1<<6) | (1<<5) | (0<<3);
		break;
	}
	sendData(madctl);
}

void lcdST7735R_setRasterHorizontal()
{
	sendCmd(ST7735_MADCTL);  // memory access control (directions)
	uint8_t madctl;
	switch(MOUNT_ROTATE){
	case LCD_ST7735R_ROTATE_0:
		madctl = (0<<7) | (0<<6) | (0<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_90:
		madctl = (1<<7) | (0<<6) | (1<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_270:
		madctl = (0<<7) | (1<<6) | (1<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_180:
		madctl = (1<<7) | (1<<6) | (0<<5) | (0<<3);
		break;
	}
	sendData(madctl);
}


/*** Internal Function Definitions ***/
static void sendCmd(uint8_t cmd)
{
	SET_LCD_CMD;
	ENABLE_LCD_CS;
	spiSendBlocking(cmd);
	DISABLE_LCD_CS;
}

static void sendData(uint8_t data)
{
	SET_LCD_DATA;
	ENABLE_LCD_CS;
	spiSendBlocking(data);
	DISABLE_LCD_CS;
}

static void intLcd()
{
	sendCmd(ST7735_SWRESET);	_delay_ms(1);
	sendCmd(ST7735_SLPOUT);	_delay_ms(1);
	sendCmd(ST7735_FRMCTR1);	_delay_ms(1);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);
	sendCmd(ST7735_FRMCTR2);
	sendData(0x01);
	sendData(0x2C);
	sendData(0x2D);
	
	sendCmd(ST7735_FRMCTR3);
	sendData(0x01); // dot inversion mode
	sendData(0x2C);
	sendData(0x2D);
	sendData(0x01); // line inversion mode
	sendData(0x2C);
	sendData(0x2D);
	
	sendCmd(ST7735_INVCTR);  // display inversion control
	sendData(0x07);  // no inversion

	sendCmd(ST7735_PWCTR1);  // power control
	sendData(0xA2);
	sendData(0x02);      // -4.6V
	sendData(0x84);      // AUTO mode

	sendCmd(ST7735_PWCTR2);  // power control
	sendData(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

	sendCmd(ST7735_PWCTR3);  // power control
	sendData(0x0A);      // Opamp current small
	sendData(0x00);      // Boost frequency

	sendCmd(ST7735_PWCTR4);  // power control
	sendData(0x8A);      // BCLK/2, Opamp current small & Medium low
	sendData(0x2A);

	sendCmd(ST7735_PWCTR5);  // power control
	sendData(0x8A);
	sendData(0xEE);

	sendCmd(ST7735_VMCTR1);  // power control
	sendData(0x0E);

	sendCmd(ST7735_INVOFF);    // don't invert display

	sendCmd(ST7735_MADCTL);  // memory access control (directions)
	uint8_t madctl;
	switch(MOUNT_ROTATE){
	case LCD_ST7735R_ROTATE_0:
		madctl = (0<<7) | (0<<6) | (0<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_90:
		madctl = (1<<7) | (0<<6) | (1<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_270:
		madctl = (0<<7) | (1<<6) | (1<<5) | (0<<3);
		break;
	case LCD_ST7735R_ROTATE_180:
		madctl = (1<<7) | (1<<6) | (0<<5) | (0<<3);
		break;
	}
	sendData(madctl);
	
	
	sendCmd(ST7735_COLMOD);  // set color mode
	sendData(0x05);        // 16-bit color

	sendCmd(ST7735_CASET);  // column addr set
	sendData(0x00);
	sendData(0x00);   // XSTART = 0
	sendData(0x00);
	sendData(LCD_ST7735R_WIDTH);   // XEND = 127

	sendCmd(ST7735_RASET);  // row addr set
	sendData(0x00);
	sendData(0x00);    // XSTART = 0
	sendData(0x00);
	sendData(LCD_ST7735R_HEIGHT);    // XEND = 159

	sendCmd(ST7735_GMCTRP1);
	sendData(0x0f);
	sendData(0x1a);
	sendData(0x0f);
	sendData(0x18);
	sendData(0x2f);
	sendData(0x28);
	sendData(0x20);
	sendData(0x22);
	sendData(0x1f);
	sendData(0x1b);
	sendData(0x23);
	sendData(0x37);
	sendData(0x00);
	sendData(0x07);
	sendData(0x02);
	sendData(0x10);
	sendCmd(ST7735_GMCTRN1);
	sendData(0x0f);
	sendData(0x1b);
	sendData(0x0f);
	sendData(0x17);
	sendData(0x33);
	sendData(0x2c);
	sendData(0x29);
	sendData(0x2e);
	sendData(0x30);
	sendData(0x30);
	sendData(0x39);
	sendData(0x3f);
	sendData(0x00);
	sendData(0x07);
	sendData(0x03);
	sendData(0x10);
	
	sendCmd(ST7735_DISPON);
	_delay_ms(1);

	sendCmd(ST7735_NORON);  // normal display on
	_delay_ms(1);
	
	sendCmd(ST7735_DISPON);
}
