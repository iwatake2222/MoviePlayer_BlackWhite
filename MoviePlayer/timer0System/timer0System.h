/*
 * timer0System.h
 *
 * Created: 2016-03-03 7:26:19 PM
 *  Author: take-iwiw
 */ 


#ifndef TIMER0SYSTEM_H_
#define TIMER0SYSTEM_H_

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
void timer0SystemInit();
uint8_t timer0SystemGetTimeMS();
RET timer0SetCallback(void (*func)());
RET timer0ClearCallback(void (*func)());

#endif /* TIMER0SYSTEM_H_ */