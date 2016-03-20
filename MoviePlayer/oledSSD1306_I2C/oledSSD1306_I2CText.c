/*
 * oledSSD1306_I2CText.c
 *
 * Created: 2016-03-19 7:59:04 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "../font/font.h"

#include "oledSSD1306_I2C.h"
#include "oledSSD1306_I2CConfig.h"

/*** Internal Const Values, Macros ***/

/*** Internal Static Variables ***/
// x, y is pixel
static uint8_t s_currentPosX = TEXT_AREA_ORIGIN_X;
static uint8_t s_currentPosY = TEXT_AREA_ORIGIN_Y;
static uint8_t s_savedPosX;
static uint8_t s_savedPosY;
/*** Internal Function Declarations ***/
static void oledSSD1306_drawChar(char c);

/*** External Function Defines ***/
void oledSSD1306_setCurPos(uint8_t textX, uint8_t textY)
{
	s_currentPosX = TEXT_AREA_ORIGIN_X + textX * (FONT_WIDTH * FONT_DISPLAY_SIZE);
	s_currentPosY = TEXT_AREA_ORIGIN_Y + textY * (FONT_HEIGHT * FONT_DISPLAY_SIZE);
	if( s_currentPosX > TEXT_AREA_END_X ) {
		s_currentPosX = TEXT_AREA_ORIGIN_X;
	}
	if( s_currentPosY > TEXT_AREA_END_Y ) {
		s_currentPosY = TEXT_AREA_ORIGIN_Y;
	}
}

void oledSSD1306_saveCurPos()
{
	s_savedPosX = s_currentPosX;
	s_savedPosY = s_currentPosY;
}

void oledSSD1306_loadCurPos()
{
	s_currentPosX = s_savedPosX;
	s_currentPosY = s_savedPosY;
}

void oledSSD1306_putchar(char c)
{
	uint8_t endX = s_currentPosX + FONT_WIDTH * FONT_DISPLAY_SIZE;
	uint8_t endY = s_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
	if( endX > TEXT_AREA_END_X ) {
		s_currentPosX = TEXT_AREA_ORIGIN_X;
		endX = s_currentPosX + FONT_WIDTH * FONT_DISPLAY_SIZE;
		s_currentPosY += FONT_HEIGHT * FONT_DISPLAY_SIZE;
		endY = s_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
		if( endY > TEXT_AREA_END_Y ) {
			s_currentPosY = TEXT_AREA_ORIGIN_Y;
			endY = s_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
		}
		oledSSD1306_fillRect(0, TEXT_AREA_ORIGIN_X, s_currentPosY, TEXT_AREA_END_X, endY);
	}

	oledSSD1306_setAddrWindow(s_currentPosX, s_currentPosY, endX, endY);
	oledSSD1306_drawChar(c);

	s_currentPosX = endX;
}

static void oledSSD1306_drawChar(char c) {
	for (uint8_t i =0; i<FONT_WIDTH; i++ ) {	
		for(uint8_t sizeX = 0; sizeX < FONT_DISPLAY_SIZE; sizeX++){			
			uint8_t line = pgm_read_byte(font+(c*FONT_WIDTH)+i);	
			uint8_t data = 0;
			uint8_t dataDigit = 0;
			for(uint8_t lineDigit = 0; lineDigit < 8; )	{
				for(uint8_t sizeY = 0; sizeY < FONT_DISPLAY_SIZE; sizeY++) {
					dataDigit++;
					data >>= 1;
					if (line & 0x01) data+=0x80;
				}
				line >>= 1;
				lineDigit++;
				if(dataDigit==8) {
					oledSSD1306_sendDataByte(data);
					data = 0;
					dataDigit = 0;
				}
			}
		}
	}
}
