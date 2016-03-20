/*
 * charLcd.c
 *
 * Created: 2016-03-02 7:35:31 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>

#include "charLcdConfig.h"
#include "charLcd.h"

/*** Internal Const Values ***/
/*** Internal Static Variables ***/
static uint8_t s_charLcdCurrentRow = 0;
static uint8_t s_charLcdCurrentCol = 0;
static uint8_t s_savedPosX = 0;
static uint8_t s_savedPosY = 0;

/*** Internal Function Declarations ***/
static void charLcdPortInit();
static void charLcdLatch();
static void charLcdSendCmd(uint8_t data);
static void charLcdSendData(uint8_t data);

/*** External Function Defines ***/
void charLcdInit()
{
	_delay_ms(15);
	charLcdPortInit();
		
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, 0x03);
	charLcdLatch();
	_delay_ms(4);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, 0x03);
	charLcdLatch();
	_delay_us(100);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, 0x03);
	charLcdLatch();
	_delay_us(40);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, 0x02);	// enter 4-bit mode
	charLcdLatch();
	_delay_us(40);
	charLcdSendCmd(0x28);	// set 2 lines mode
	_delay_us(40);
	charLcdSendCmd(0x0F);
	_delay_us(40);
	charLcdSendCmd(0x01);	// Display Clear
	_delay_ms(2);
	charLcdSendCmd(0x06);
	_delay_us(40);
	charLcdSendCmd(0x80);
	_delay_us(40);

}

void charLcdPosition(const uint8_t x, const uint8_t y)
{
	//uint8_t add = 0x40 * y - (2<=y ? (0x40*2-0x14) : 0) + x;
	uint8_t add = x + (y == 0 ? 0 : 0x40);
	s_charLcdCurrentCol = x;
	s_charLcdCurrentRow = y;
	
	charLcdSendCmd(0x80 + add);
	_delay_us(40);
}

void charLcdSavePosition()
{
	s_savedPosX = s_charLcdCurrentCol;
	s_savedPosY = s_charLcdCurrentRow;
}
void charLcdLoadPosition()
{
	charLcdPosition(s_savedPosX, s_savedPosY);
}

void charLcdClear()
{
	charLcdSendCmd(0x01);	// Display Clear
	_delay_ms(2);
	s_charLcdCurrentRow = 0;
	s_charLcdCurrentCol = 0;
}

void charLcdPutchar(const uint8_t c)
{
	if(s_charLcdCurrentCol >= CHAR_LCD_COL){
		s_charLcdCurrentCol = 0;
		s_charLcdCurrentRow++;
		if(s_charLcdCurrentRow >= CHAR_LCD_ROW){
			s_charLcdCurrentRow = 0;
		}
		charLcdPosition(s_charLcdCurrentCol, s_charLcdCurrentRow);
		for(int i=0; i<CHAR_LCD_COL;i++){
			charLcdSendData(' ');
		}
		charLcdPosition(s_charLcdCurrentCol, s_charLcdCurrentRow);
	}
	charLcdSendData(c);
	s_charLcdCurrentCol++;
}

/*** Internal Function Definitions ***/
static void charLcdPortInit()
{
	SET_BIT(CHAR_LCD_EN_DDR, CHAR_LCD_EN_BIT);
	SET_BIT(CHAR_LCD_RS_DDR, CHAR_LCD_RS_BIT);
	SET_VAL(CHAR_LCD_D_DDR, CHAR_LCD_D_BIT, 4, 0x0F);
	CLR_BIT(CHAR_LCD_EN_PORT, CHAR_LCD_EN_BIT);
	CLR_BIT(CHAR_LCD_RS_PORT, CHAR_LCD_RS_BIT);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, 0x00);

}

static void charLcdLatch()
{
	CLR_BIT(CHAR_LCD_EN_PORT, CHAR_LCD_EN_BIT);
	SET_BIT(CHAR_LCD_EN_PORT, CHAR_LCD_EN_BIT);
	_delay_us(1);
	CLR_BIT(CHAR_LCD_EN_PORT, CHAR_LCD_EN_BIT);
}

static void charLcdSendCmd(uint8_t data)
{
	CLR_BIT(CHAR_LCD_RS_PORT, CHAR_LCD_RS_BIT);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, (data>>4)&0x0f);
	charLcdLatch();
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, data&0x0f);
	charLcdLatch();
	_delay_us(40);
}

static void charLcdSendData(uint8_t data)
{
	SET_BIT(CHAR_LCD_RS_PORT, CHAR_LCD_RS_BIT);
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, (data>>4)&0x0f);
	charLcdLatch();
	SET_VAL(CHAR_LCD_D_PORT, CHAR_LCD_D_BIT, 4, data&0x0f);
	charLcdLatch();
	_delay_us(40);
}
