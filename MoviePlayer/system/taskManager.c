/*
 * taskManager.c
 *
 * Created: 2016-03-13 10:55:51 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include "taskManager.h"

/*** Internal Const Values ***/
//#define IS_MEASURE_PROCESSING_TIME

/*** Internal Static Variables ***/
static void (*s_taskInit[MAX_TASK])();
static void (*s_taskLoop[MAX_TASK])();
static uint8_t s_maxProcessingTime;
static uint8_t s_currentProcessingTime;

/*** Internal Function Declarations ***/

/*** External Function Defines ***/
RET registerInit(void (*func)())
{
	static int8_t s_numInit = 0;
	if(s_numInit >= MAX_TASK) return RET_ERR_REGISTER;
	s_taskInit[s_numInit] = func;
	s_numInit++;
	return RET_OK;
}

RET registerLoop(void (*func)())
{
	static int8_t s_numLoop = 0;
	if(s_numLoop >= MAX_TASK) return RET_ERR_REGISTER;
	s_taskLoop[s_numLoop] = func;
	s_numLoop++;
	return RET_OK;
}

void systemInit()
{
	for(uint8_t i = 0; i < MAX_TASK; i++) {
		if(s_taskInit[i] != 0) s_taskInit[i]();
		else break;
	}
	
}

void systemLoop()
{
	while(1) {	
#ifdef IS_MEASURE_PROCESSING_TIME		
		uint16_t startTime = getTimeMS();
#endif
		for(uint8_t i = 0; i < MAX_TASK; i++) {
			if(s_taskLoop[i] != 0) s_taskLoop[i]();
			else break;
		}

#ifdef IS_MEASURE_PROCESSING_TIME
		uint16_t finishTime = getTimeMS();
		uint8_t interval = 0;
		if(finishTime > startTime) {
			interval =  (finishTime - startTime)&0xff;
			} else {
			interval =  ((255 - startTime) + finishTime + 1)&0xff;	// to avoid using more than 16bit
		}
		if(interval > s_maxProcessingTime) s_maxProcessingTime = interval;
		s_currentProcessingTime = interval;
		//printHex(s_maxProcessingTime);
		//printHex(s_currentProcessingTime);
#endif
	}	
}

uint8_t getMaxProcessingTime()
{
	return s_maxProcessingTime;
}

uint8_t getCurrentProcessingTime()
{
	return s_currentProcessingTime;
}
