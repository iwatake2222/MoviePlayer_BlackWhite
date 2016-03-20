/*
 * fpsCheck.c
 *
 * Created: 2016-03-05 3:36:18 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "../myLibraries/myStdio.h"
#include "../myLibraries/myTimer.h"
#include "../myLibraries/myVideo.h"
#include "../color/colorRGB.h"

/*** Internal Const Values ***/
#define WIDTH 128
#define HEIGHT 128

/*** Internal Static Variables ***/
/*** Internal Function Declarations ***/
void drawDormal();
void drawWithBuffer();

/*** External Function Defines ***/
void fpsCheckInit()
{
	
}

void fpsCheckLoop()
{
	drawDormal();
	//drawWithBuffer();
}

/*** Internal Function Definitions ***/
void drawDormal()
{
	static uint8_t colorSwitch = 0;
	setVideoArea(0, 0, WIDTH, HEIGHT);
	startFrame();
	suspendDraw();
	for(uint8_t y = 0; y < HEIGHT; y++){
		for(uint8_t x = 0; x < WIDTH; x++){
			resumeDraw();
			if(colorSwitch == 0){drawPixel(0x00, 0x1F); colorSwitch=1;}
			else {drawPixel(0x00, 0x00); colorSwitch=0;}
			suspendDraw();
		}
	}
	uint8_t time = stopFrame();
	printDecPos(1000/time, 0, 0);
}

void drawWithBuffer()
{
	static uint8_t colorSwitch = 0;
	setVideoArea(0, 0, WIDTH, HEIGHT);
	startFrame();
	suspendDraw();
	uint8_t buffer[WIDTH*2];
	for(uint16_t x=0; x<WIDTH*2; ){
		if(colorSwitch == 0){buffer[x++] = 0x00; buffer[x++] = 0x1f; colorSwitch=1;}
		else {buffer[x++] = 0x00; buffer[x++] = 0x00; colorSwitch=0;}
	}
	for(uint8_t y = 0; y < HEIGHT; y++){
		resumeDraw();
		drawBuffer(buffer, WIDTH*2);
		suspendDraw();
	}
	uint8_t time = stopFrame();
	printDecPos(1000/time, 0, 0);
}


