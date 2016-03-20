/*
 * myTimer.h
 *
 * Created: 2016-03-03 7:34:04 PM
 *  Author: take-iwiw
 */ 


#ifndef MYTIMER_H_
#define MYTIMER_H_

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
void timerInit();
void timerLoop();
uint16_t getTimeMS();
uint16_t getIntervalTime();

// callback runs on main loop context
RET setIntervalTimer(void (*func)(), uint8_t intervalMS);
RET clearIntervalTimer(void (*func)());

// callback runs on timer interrupt handler
RET setIntervalTimerHW(void (*func)(), uint8_t intervalMS);
RET clearIntervalTimerHW(void (*func)());

#endif /* MYTIMER_H_ */