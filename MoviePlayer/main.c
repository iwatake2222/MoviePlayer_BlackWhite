/*
 * main.c
 *
 * Created: 2016-03-02 4:15:28 PM
 * Author : take-iwiw
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "myCommon.h"
#include <util/delay.h>

#include "./system/taskManager.h"
#include "./myLibraries/myStdio.h"
#include "./myLibraries/myTimer.h"
#include "./myLibraries/myVideo.h"
#include "./myLibraries/myInput.h"
#include "./application/application.h"

/*** Internal Const Values ***/

/*** Internal Static Variables ***/

/*** Internal Function Declarations ***/
static void librariesInit();
static void librariesLoop();
static void testEchoBack();

/*** External Function Defines ***/
int main(void)
{
	_delay_ms(100);
	/* register tasks */
	registerInit(librariesInit);
	registerLoop(librariesLoop);
	registerLoop(testEchoBack);
	//registerInit(deviceTrialInit);
	//registerLoop(deviceTrialLoop);
	//registerInit(fpsCheckInit);
	//registerLoop(fpsCheckLoop);
	registerInit(moviePlayerInit);
	registerLoop(moviePlayerLoop);
	//registerInit(tinyDisplayInit);
	//registerLoop(tinyDisplayLoop);

	/* call all initializations */
	systemInit();
	print("Hello");

	/* call main loop */
	print("Start");
	systemLoop();	
}


/*** Internal Function Definitions ***/
static void librariesInit()
{
	sei();	//SET_BIT(SREG, SREG_I);
	timerInit();
	stdioInit();
	inputInit();
	videoInit();	
}

static void librariesLoop()
{
	timerLoop();
}


static void testEchoBack()
{
	char c;
	if(getcharTry(&c) == RET_OK)putchar(c);

	//printDec(getCurrentProcessingTime());		
}

