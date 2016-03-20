/*
 * lcdST7735R_Text.c
 *
 * Created: 2016-03-04 7:30:19 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "../font/font.h"

#include "lcdST7735R_SPI.h"
#include "lcdST7735R_SPIConfig.h"


static void lcdST7735R_drawChar(char c);

// x, y is pixel
static uint8_t s_currentPosX = TEXT_AREA_ORIGIN_X;
static uint8_t s_currentPosY = TEXT_AREA_ORIGIN_Y;
static uint8_t s_savedPosX;
static uint8_t s_savedPosY;


void lcdST7735R_setCurPos(uint8_t textX, uint8_t textY)
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

void lcdST7735R_saveCurPos()
{
	s_savedPosX = s_currentPosX;
	s_savedPosY = s_currentPosY;
}

void lcdST7735R_loadCurPos()
{
	s_currentPosX = s_savedPosX;
	s_currentPosY = s_savedPosY;
}


void lcdST7735R_putchar(char c)
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
		lcdST7735R_fillRect(TEXT_AREA_BACKGROUND, TEXT_AREA_ORIGIN_X, s_currentPosY, TEXT_AREA_END_X, endY);
	}
#ifdef IS_FONT_ROTATE
	lcdST7735R_setRasterVertical();
	lcdST7735R_setAddrWindow(s_currentPosY, s_currentPosX, endY, endX);
	lcdST7735R_startSendBurstData();
	lcdST7735R_drawChar(c);
	lcdST7735R_stopSendBurstData();
	lcdST7735R_setRasterHorizontal();
#else
	lcdST7735R_setAddrWindow(s_currentPosX, s_currentPosY, endX, endY);
	lcdST7735R_startSendBurstData();
	lcdST7735R_drawChar(c);
	lcdST7735R_stopSendBurstData();
#endif
	s_currentPosX = endX;	
}

static void lcdST7735R_drawChar(char c) {
	for (uint8_t i =0; i<FONT_WIDTH; i++ ) {
		for(uint8_t size = 0; size < FONT_DISPLAY_SIZE; size++){
			uint8_t line = pgm_read_byte(font+(c*FONT_WIDTH)+i);
			for (uint8_t j = 0; j<8; j++) {
				for(uint8_t size = 0; size < FONT_DISPLAY_SIZE; size++){
					if (line & 0x1) {
						lcdST7735R_sendBurstData(TEXT_AREA_FOREGROUND >> 8, TEXT_AREA_FOREGROUND);
					} else {
						lcdST7735R_sendBurstData(TEXT_AREA_BACKGROUND >> 8, TEXT_AREA_BACKGROUND);
					}
				}
				line >>= 1;
			}
		}
	}
}
