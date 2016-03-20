/*
 * rotaryEncoder.c
 *
 * Created: 2016-03-09 3:59:52 PM
 * Author: take-iwiw
 *   Note: Need to use HW timer to check io
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "rotaryEncoderConfig.h"
#include "rotaryEncoder.h"
#include "../myLibraries/myTimer.h"

/*** Internal Const Values ***/
/*** Internal Static Variables ***/
static uint8_t s_previousCode;
static uint8_t s_currentCode;
volatile static int8_t s_sumRotation = 0;

/*** Internal Function Declarations ***/
static void checkCodeBeforeChattering();
static void checkCode();

/*** External Function Defines ***/
void rotaryEncoder_init()
{
	CLR_BIT(ROTARY_ENCODER_A_DDR, ROTARY_ENCODER_A_BIT);
	CLR_BIT(ROTARY_ENCODER_B_DDR, ROTARY_ENCODER_B_BIT);
#ifdef ROTARY_ENCODER_USE_PULLUP
	SET_BIT(PORTC, 0);
	SET_BIT(PORTC, 1);
#endif
	
	s_currentCode = IS_SET(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_A_BIT)<<1 | IS_SET(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_B_BIT);
	s_previousCode = s_currentCode;
	
	setIntervalTimerHW(checkCodeBeforeChattering, ROTARY_ENCODER_CHECK_INTERVAL_MS);
}

int8_t getResetRotaryEncoderRotationTick()
{
	int8_t ret = s_sumRotation;
	s_sumRotation = 0;
	return ret;	
}


/*** Internal Function Definitions ***/
static void checkCodeBeforeChattering()
{
	s_currentCode = IS_SET(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_A_BIT)<<1 | IS_SET(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_B_BIT);
	if( s_previousCode != s_currentCode  ){
		setIntervalTimerHW(checkCode, ROTARY_ENCODER_CHECK_CHATTERING_MS);
	}
}


/*
 * code(FW) = 0 1 3 2 0 1 3 2 0 1 3 2
 * previous now   result
 *   0       0     STOP
 *   0       1     FW
 *   0       2     BW
 *   0       3     -
 *   1       0     BW
 *   1       1     STOP
 *   1       2     -
 *   1       3     FW
 *   2       0     FW
 *   2       1     -
 *   2       2     STOP
 *   2       3     BW
 *   3       0     -
 *   3       1     BW
 *   3       2     FW
 *   3       3     STOP
 */
static void checkCode()
{
	clearIntervalTimerHW(checkCode);
	
	if(s_currentCode != (IS_SET(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_A_BIT)<<1 | IS_SET(ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_B_BIT))) return;
	//printDec(s_current);
	uint8_t codeTransit = s_previousCode << 2 | s_currentCode;
	static const int rotationTable[] = { 0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0 };
	int8_t rotation = rotationTable[codeTransit & 15];
	s_previousCode = s_currentCode;
	s_sumRotation += rotation;
	//printDec(s_sumRotation);
}

