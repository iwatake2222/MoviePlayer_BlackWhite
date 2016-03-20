/*
 * oledSSD1306_I2C.c
 *
 * Created: 2016-03-19 7:29:04 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "oledSSD1306_I2CConfig.h"
#include "oledSSD1306_I2C.h"
#include "oledSSD1306_CMD.h"
#include "../i2c/i2c.h"

/*** Internal Const Values, Macros ***/
#define CMD  0x00
#define DATA 0x40

/*** Internal Static Variables ***/
uint8_t s_isInit = 0;
/*** Internal Function Declarations ***/
inline static void sendCommand(uint8_t data)
{
	i2cWriteMaster(SLAVE_ADDR, CMD, 1, &data);
}

/*** External Function Defines ***/
void oledSSD1306_init()
{
	if(s_isInit != 0) return;
	s_isInit = 1;
	
	oledSSD1306_start();

	/* init sequence is from datasheet */
	sendCommand(SSD1306_DISPLAYOFF);
	sendCommand(SSD1306_SETMULTIPLEX);
	sendCommand(0x3F);
	sendCommand(SSD1306_SETDISPLAYOFFSET);
	sendCommand(0x00);
	sendCommand(SSD1306_SETSTARTLINE | 0x00);
	sendCommand(SSD1306_SEGREMAP | 0x01);
	sendCommand(SSD1306_COMSCANDEC);
	sendCommand(SSD1306_SETCOMPINS);
	sendCommand(0x12);
	sendCommand(SSD1306_SETCONTRAST);
	sendCommand(0xCF);
	sendCommand(SSD1306_DISPLAYALLON_RESUME);
	sendCommand(SSD1306_NORMALDISPLAY);
	sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
	sendCommand(0x80);
	sendCommand(SSD1306_CHARGEPUMP);
	sendCommand(0x14);
	sendCommand(SSD1306_MEMORYMODE);
	sendCommand(0x01);	// vertical ddressing mode
	sendCommand(SSD1306_DISPLAYON);
	
	//oledSSD1306_setAddrWindowFull();
	oledSSD1306_fillRect(0, 0, 0, OLED_SSD1306_WIDTH, OLED_SSD1306_HEIGHT);
	
	oledSSD1306_stop();
}

// y must be 8 align
void oledSSD1306_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	oledSSD1306_start();
	sendCommand(SSD1306_COLUMNADDR);
	sendCommand(x0);
	sendCommand(x1 - 1);
	sendCommand(SSD1306_PAGEADDR);
	sendCommand(y0/OLED_SSD1306_PAGE_SIZE);
	sendCommand(y1/OLED_SSD1306_PAGE_SIZE - 1);
	oledSSD1306_stop();
}

inline void oledSSD1306_setAddrWindowFull()
{
	oledSSD1306_setAddrWindow(0, 0, OLED_SSD1306_WIDTH, OLED_SSD1306_HEIGHT);
}

// y must be 8 align
void oledSSD1306_fillRect(uint16_t color, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t width = x1 - x0;
	uint8_t yStart = y0/OLED_SSD1306_PAGE_SIZE;
	uint8_t yEnd = y1/OLED_SSD1306_PAGE_SIZE;

	oledSSD1306_setAddrWindow(x0, y0, x1, y1);
	oledSSD1306_start();
	uint8_t data = color==0?0:0xff;
	for(uint8_t x = x0; x < x1; x++){
		for(uint8_t y = yStart; y < yEnd; y++){			
			oledSSD1306_sendDataByte(data);
		}
	}
	oledSSD1306_stop();
	oledSSD1306_setAddrWindowFull();
}

inline void oledSSD1306_sendDataByte(uint8_t data)
{
	i2cWriteMaster(SLAVE_ADDR, DATA, 1, &data);	
}

inline void oledSSD1306_sendDataBurst(uint8_t *data, uint16_t num)
{
	i2cWriteMaster(SLAVE_ADDR, DATA, num, data);
}

inline void oledSSD1306_start()
{
	I2C_OPEN_PRM prm;
	prm.sclKHz = I2C_SPEED_KHZ;
	i2cOpen(&prm);
}

inline void oledSSD1306_stop()
{
	i2cClose();
}
