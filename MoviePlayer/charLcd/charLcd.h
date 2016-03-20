/*
 * charLcd.h
 *
 * Created: 2016-03-02 7:35:41 PM
 *  Author: take-iwiw
 */ 


#ifndef CHARLCD_H_
#define CHARLCD_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/
void charLcdInit();
void charLcdPutchar(const uint8_t c);
void charLcdPosition(const uint8_t x, const uint8_t y);
void charLcdSavePosition();
void charLcdLoadPosition();
void charLcdClear();


#endif /* LCD_H_ */