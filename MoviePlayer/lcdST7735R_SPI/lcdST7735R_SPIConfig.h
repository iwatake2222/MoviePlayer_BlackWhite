/*
 * lcdST7735R_SPIConfig.h
 *
 * Created: 2016-03-03 11:22:27 PM
 *  Author: take-iwiw
 */ 


#ifndef LCDST7735R_SPICONFIG_H_
#define LCDST7735R_SPICONFIG_H_

/*** definitions (do not modify this section) ***/
#define LCD_ST7735R_ROTATE_0 0
#define LCD_ST7735R_ROTATE_90 1
#define LCD_ST7735R_ROTATE_180 2
#define LCD_ST7735R_ROTATE_270 3


/*** configure (modify this section depending on environment) ***/
/* io settings */
#define LCD_ST7735R_SPI_CS_PORT PORTB
#define LCD_ST7735R_SPI_DC_PORT PORTB
#define LCD_ST7735R_SPI_CS_DDR DDRB
#define LCD_ST7735R_SPI_DC_DDR DDRB
#define LCD_ST7735R_SPI_CS_BIT 2
#define LCD_ST7735R_SPI_DC_BIT 1

/* physical mount settings */
#define MOUNT_ROTATE (LCD_ST7735R_ROTATE_180)
// do not modify here
#if (MOUNT_ROTATE == LCD_ST7735R_ROTATE_0) || (MOUNT_ROTATE == LCD_ST7735R_ROTATE_180)
#define LCD_ST7735R_WIDTH 128
#define LCD_ST7735R_HEIGHT 160
#else
#define LCD_ST7735R_WIDTH 160
#define LCD_ST7735R_HEIGHT 128
#endif

/* text settings */
#define TEXT_AREA_ORIGIN_X 0
// for video
#define TEXT_AREA_ORIGIN_Y 128	
//#define TEXT_AREA_ORIGIN_Y 0
// actual end position is the below number -1
#define TEXT_AREA_END_X 128
#define TEXT_AREA_END_Y 160
#define IS_FONT_ROTATE
#define FONT_WIDTH 5
#define FONT_HEIGHT 8
#define FONT_DISPLAY_SIZE 2
#define TEXT_AREA_BACKGROUND (0x0000)
#define TEXT_AREA_FOREGROUND ((uint16_t)0x001F)








#endif /* LCDST7735R_SPICONFIG_H_ */