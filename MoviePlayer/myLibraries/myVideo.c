/*
 * myVideo.c
 *
 * Created: 2016-03-05 2:22:51 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include "myRetarget.h"
#include "myVideo.h"
#include "myTimer.h"

#ifdef USE_LCD_ST7735R_VIDEO
#include "../lcdST7735R_SPI/lcdST7735R_SPI.h"
#endif

#ifdef USE_OLED_SSD1306_VIDEO
#include "../oledSSD1306_I2C/oledSSD1306_I2C.h"
#endif

/*** Internal Const Values ***/

/*** Internal Static Variables ***/
static uint8_t s_width;
static uint8_t s_height;
static uint16_t s_frameStartTimeMS;


/*** Internal Function Declarations ***/
/*** External Function Defines ***/
void videoInit()
{
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_init();
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	oledSSD1306_init();
#endif
}

void setVideoArea(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height)
{
	s_width = width;
	s_height = height;
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_setAddrWindow(x0, y0, x0 + width, y0 + height);
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	oledSSD1306_setAddrWindow(x0, y0, x0 + width, y0 + height);
#endif
}

void startFrame()
{
	s_frameStartTimeMS = getTimeMS();
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_startSendBurstData();
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	oledSSD1306_start();
#endif
}

uint16_t stopFrame()
{
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_stopSendBurstData();
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	oledSSD1306_stop();
#endif
	// check if frame skip happens
	uint16_t frameEndTimeMS = getTimeMS();
	uint16_t realFrameTime;
	if(frameEndTimeMS > s_frameStartTimeMS) {
		realFrameTime = (frameEndTimeMS - s_frameStartTimeMS)&0xffff;
		} else {
		realFrameTime = ((65535 - s_frameStartTimeMS) + frameEndTimeMS + 1)&0xffff;	// to avoid using more than 16bit
	}
	return realFrameTime;
}

inline void suspendDraw()
{
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_pauseSendBurstData();
#endif
}

inline void resumeDraw()
{
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_restartSendBurstData();
#endif
}

inline void drawPixel(uint8_t dataHigh, uint8_t dataLow)
{
#ifdef USE_LCD_ST7735R_VIDEO
	lcdST7735R_sendBurstData(dataHigh, dataLow);
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	static uint8_t s_accumulatedDrawPixel8 = 0;
	static uint8_t s_accumulatedDrawPixelNum = 0;
	if(dataHigh !=0 || dataLow != 0) s_accumulatedDrawPixel8 += 1<< s_accumulatedDrawPixelNum;
	s_accumulatedDrawPixelNum++;
	if(s_accumulatedDrawPixelNum == 8){
		oledSSD1306_sendDataByte(s_accumulatedDrawPixel8);
		s_accumulatedDrawPixel8 = 0;
		s_accumulatedDrawPixelNum = 0;
	}
#endif
}

void drawBuffer(uint8_t *buffer, uint16_t size)
{

#ifdef USE_LCD_ST7735R_VIDEO
	for(uint8_t x = 0; x < size/2; x++ ) {
		lcdST7735R_sendBurstData(*buffer, *(buffer+1));
		buffer += 2;
	}
#endif
#ifdef USE_OLED_SSD1306_VIDEO
	oledSSD1306_sendDataBurst(buffer, size);
#endif

}


/*** Internal Function Definitions ***/
