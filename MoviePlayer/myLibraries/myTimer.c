/*
 * myTimer.c
 *
 * Created: 2016-03-03 7:33:53 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include "myRetarget.h"
#include "myTimer.h"
#include "myStdio.h"

#ifdef USE_TIMER0_SYSTEM
#include "../timer0System/timer0System.h"
#endif

/*** Internal Const Values ***/
#define TIMER_INTERVAL_CALLBACK_MAX 8
#define TIMER_INTERVAL_CALLBACK_HW_MAX 4

/*** Internal Static Variables ***/
static void (*s_intervalCallback[TIMER_INTERVAL_CALLBACK_MAX])();
static uint8_t s_intervalTimeCount[TIMER_INTERVAL_CALLBACK_MAX];
static volatile  uint8_t s_intervalTime[TIMER_INTERVAL_CALLBACK_MAX];

static void (*s_intervalCallbackHW[TIMER_INTERVAL_CALLBACK_HW_MAX])();
static uint8_t s_intervalTimeCountHW[TIMER_INTERVAL_CALLBACK_HW_MAX];
static volatile  uint8_t s_intervalTimeHW[TIMER_INTERVAL_CALLBACK_HW_MAX];

/*** Internal Function Declarations ***/
static void timerCallbackHW();

/*** External Function Defines ***/
void timerInit()
{
#ifdef USE_TIMER0_SYSTEM
	timer0SystemInit();
	timer0SetCallback(timerCallbackHW);
#endif

}

uint16_t getTimeMS()
{
#ifdef USE_TIMER0_SYSTEM
	return timer0SystemGetTimeMS();
#endif
	return 1;
}

uint16_t getIntervalTime()
{
	static uint16_t s_lastCntTimer0_1ms;
	uint16_t now = getTimeMS();
	uint16_t previous = s_lastCntTimer0_1ms;
	s_lastCntTimer0_1ms = now;
	if(now > previous) {
		return (now - previous)&0xffff;
	} else {
		//return (1 << 16) - previous + now;
		return ((65535 - previous) + now + 1)&0xffff;	// to avoid using more than 16bit
	}
}

RET setIntervalTimer(void (*func)(), uint8_t intervalMS)
{
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_MAX; i++) {
		if(s_intervalTime[i] == 0){
			s_intervalCallback[i] = func;
			s_intervalTime[i] = intervalMS;
			s_intervalTimeCount[i] = 0;
			return RET_OK;
		}
	}
	print("E_timer");
	return RET_ERR_REGISTER;
}

RET clearIntervalTimer(void (*func)())
{
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_MAX; i++) {
		if(s_intervalCallback[i] == func){
			s_intervalTime[i] = 0;
			s_intervalCallback[i] = 0;
			return RET_OK;
		}
	}
	print("E_timer");
	return RET_ERR_REGISTER;
}

RET setIntervalTimerHW(void (*func)(), uint8_t intervalMS)
{
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_HW_MAX; i++) {
		if(s_intervalTimeHW[i] == 0){
			s_intervalCallbackHW[i] = func;
			s_intervalTimeHW[i] = intervalMS;
			s_intervalTimeCountHW[i] = 0;
			return RET_OK;
		}
	}
	print("E_timer");
	return RET_ERR_REGISTER;
}

RET clearIntervalTimerHW(void (*func)())
{
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_HW_MAX; i++) {
		if(s_intervalCallbackHW[i] == func){
			s_intervalTimeHW[i] = 0;
			s_intervalCallbackHW[i] = 0;
			return RET_OK;
		}
	}
	print("E_timer");
	return RET_ERR_REGISTER;
}

void timerLoop()
{
	static uint16_t s_lastLoopTimeMS;
	uint16_t now = getTimeMS();
	uint16_t previous = s_lastLoopTimeMS;
	uint16_t interval;
	if (now >= previous) {
		interval = now - previous;
	} else {
		interval = ((65535 - previous) + now + 1)&0xffff;	// to avoid using more than 16bit
	}
	
	if(interval < 1) return;
	
	s_lastLoopTimeMS = now;
	
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_MAX; i++) {
		if(s_intervalTime[i] != 0){
			s_intervalTimeCount[i]+=interval;
			if(s_intervalTimeCount[i] >= s_intervalTime[i]){
				s_intervalTimeCount[i] = 0;
				s_intervalCallback[i]();
			}
		}
	}
}

/*** Internal Function Definitions ***/
static void timerCallbackHW()
{
	static uint16_t s_lastLoopTimeMS;
	uint16_t now = getTimeMS();
	uint16_t previous = s_lastLoopTimeMS;
	uint16_t interval;
	if (now >= previous) {
		interval = now - previous;
		} else {
		interval = ((65535 - previous) + now + 1)&0xffff;	// to avoid using more than 16bit
	}
	
	if(interval < 1) return;
	
	s_lastLoopTimeMS = now;
	
	for(uint8_t i = 0; i < TIMER_INTERVAL_CALLBACK_HW_MAX; i++) {
		if(s_intervalTimeHW[i] != 0){
			s_intervalTimeCountHW[i]+=interval;
			if(s_intervalTimeCountHW[i] >= s_intervalTimeHW[i]){
				s_intervalTimeCountHW[i] = 0;
				s_intervalCallbackHW[i]();
			}
		}
	}
}	
