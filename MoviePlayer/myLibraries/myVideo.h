/*
 * myVideo.h
 *
 * Created: 2016-03-05 2:23:04 AM
 *  Author: take-iwiw
 */ 


#ifndef MYVIDEO_H_
#define MYVIDEO_H_


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
void videoInit();
void setVideoArea(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
void startFrame();
uint16_t stopFrame();	// return actual frame time[ms]
void suspendDraw();
void resumeDraw();
void drawPixel(uint8_t dataHigh, uint8_t dataLow);
void drawBuffer(uint8_t *buffer, uint16_t size);

#endif /* MYVIDEO_H_ */