/*
 * charLcdConfig.h
 *
 * Created: 2016-03-02 7:43:47 PM
 *  Author: take-iwiw
 *    Note: Modify this file depending on wire connection
 *        : 4-bit mode fixed. Write only (w/o checking busy flag)
 */ 


#ifndef CHRALCDCONFIG_H_
#define CHRALCDCONFIG_H_

/* device settings */
static const int CHAR_LCD_ROW = 2;
static const int CHAR_LCD_COL = 16;

/* io settings */
/*
 * EN = PORTC4
 * RS = PORTC5
 * D4 = PORTC0
 * D5 = PORTC1
 * D6 = PORTC2
 * D7 = PORTC3
 */
#define CHAR_LCD_EN_PORT PORTC
#define CHAR_LCD_RS_PORT PORTC
#define CHAR_LCD_D_PORT PORTC
#define CHAR_LCD_EN_DDR DDRC
#define CHAR_LCD_RS_DDR DDRC
#define CHAR_LCD_D_DDR DDRC
#define CHAR_LCD_EN_BIT 4
#define CHAR_LCD_RS_BIT 5
#define CHAR_LCD_D_BIT 0


#endif /* CHRALCDCONFIG_H_ */