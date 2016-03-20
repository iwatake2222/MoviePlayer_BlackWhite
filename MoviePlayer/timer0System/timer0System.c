/*
 * timer0System.c
 *
 * Created: 2016-03-03 7:26:30 PM
 *  Author: take-iwiw
 *    Note: Timer0 is fixed for system timer (1 msec)
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../myCommon.h"
#include "timer0System.h"

/*** Internal Const Values ***/

/*** Internal Static Variables ***/
// over flow happens every 65.536 sec
static volatile uint16_t s_cntTimer0_1ms = 0;
static void (*s_callback)();

/*** Internal Function Declarations ***/

/*** External Function Defines ***/
void timer0SystemInit()
{
	// CTC Mode without compare output
	TCCR0A = (0<<COM0A0) | (0<<COM0B0) | (2<<WGM00);
	// Div = 1/64
	TCCR0B = 0x03;
	// Compare match every 1 msec (1/ (64/16M) / 1000)
	OCR0A = F_CPU / 64 / 1000;
	// Enable Compare Match A Interrupt
	TIMSK0 = (1<<OCIE0A);
}

RET timer0SetCallback(void (*func)())
{
	if(s_callback == 0){
		s_callback = func;
		return RET_OK;
	}
	return RET_ERR_REGISTER;
	
}

RET timer0ClearCallback(void (*func)())
{
	if(s_callback == func){
		s_callback = 0;
		return RET_OK;
	}
	return RET_ERR_REGISTER;
}

// over flow happens every 65.536 sec
uint8_t timer0SystemGetTimeMS()
{
	return s_cntTimer0_1ms;
}

/*** Internal Function Definitions ***/
ISR(TIMER0_COMPA_vect)
{
	s_cntTimer0_1ms++;
	if(s_callback != 0) s_callback();
	return;
}
