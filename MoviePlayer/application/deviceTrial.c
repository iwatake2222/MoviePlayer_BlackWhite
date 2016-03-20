/*
 * deviceTrial.c
 *
 * Created: 2016-03-09 5:10:25 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "../myLibraries/myStdio.h"
#include "../myLibraries/myTimer.h"
#include "../myLibraries/myVideo.h"
#include "../myLibraries/myInput.h"

/*** Internal Const Values ***/
/*** Internal Static Variables ***/

/*** Internal Function Declarations ***/
static void dialCallback(int8_t rotSpeedTick);
static void keysCallback(uint8_t keys);
static void positionCallback(uint8_t pos);
static void tapCallback(uint8_t tapNum);
/*** External Function Defines ***/

void deviceTrialInit()
{
	setInputDialCallback(dialCallback);
	setInputKeysCallback(keysCallback);
	setInputPositionCallback(positionCallback);
	setInputTapCallback(tapCallback);	
}

void deviceTrialLoop()
{
	printPos("ACC_X: ", 0, 4);
	printDecPos(getInputAccelX(),7, 4);
	printPos("ACC_Y: ", 0, 5);
	printDecPos(getInputAccelY(),7, 5);
	printPos("ACC_Z: ", 0, 6);
	printDecPos(getInputAccelZ(),7, 6);
}

/*** Internal Function Definitions ***/
static void dialCallback(int8_t rotSpeedTick)
{
	int limitedRotSpeed = rotSpeedTick >> 3;
	if(limitedRotSpeed == 0)return;
	printPos("Rot:        ", 0, 2);
	/* rotSpeedTick = tick / sec */
	if(rotSpeedTick < 0) {
		for(uint8_t i = 0; i < -limitedRotSpeed; i++){
			printPos(">", 7 + i, 2);
		}
	}
	for(uint8_t i = 0; i < limitedRotSpeed; i++){		
		printPos("<", 7 - i, 2);
	}
	return;
}

static void keysCallback(uint8_t keys)
{
	printPos("Keys: ", 0, 0);
	printDecPos(keys, 6, 0);
	
	// clear tap
	printPos("Tap: ", 0, 3);
	printDecPos(0, 6, 3);
}

static void positionCallback(uint8_t pos)
{
	printPos("Pos: ", 0, 1);
	printDecPos(pos, 6, 1);
}

static void tapCallback(uint8_t tapNum)
{
	if(tapNum != 0){
		printPos("Tap: ", 0, 3);
		printDecPos(tapNum, 6, 3);
	}
}
