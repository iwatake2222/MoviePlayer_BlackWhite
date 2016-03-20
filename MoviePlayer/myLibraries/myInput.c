/*
 * myInput.c
 *
 * Created: 2016-03-09 4:44:30 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include "myRetarget.h"
#include "myInput.h"
#include "myTimer.h"

#ifdef USE_DIAL_ROTARY_ENCODER
#include "../rotaryEncoder/rotaryEncoder.h"
#endif
#ifdef USE_KEYS
#include "../keys/keys.h"
#endif
#ifdef USE_POSITION_ADC
#include "../adc/adc.h"
#endif
#ifdef USE_ACCEL_ADXL345
#include "../accelADXL345/accelADXL345.h"
#endif

/*** Internal Const Values ***/
// the max value of position is 2^x
#define POSITION_MAX_VAL_EXP 4
// use 2^x to avoid div operation
#define INPUT_CHECK_INTERVAL_MS_EXP 6
#define INPUT_CHECK_INTERVAL_MS (1<<INPUT_CHECK_INTERVAL_MS_EXP)

/*** Internal Static Variables ***/
static void (*s_dialCallback)(int8_t);
static void (*s_keysCallback)(uint8_t);
static void (*s_positionCallback)(uint8_t);
static void (*s_tapCallback)(uint8_t);

/*** Internal Function Declarations ***/
static void updateInputAll();
static int8_t checkDial();
static uint8_t checkKeys();
static uint8_t checkPosition();
static uint8_t checkTap();

/*** External Function Definitions ***/
void inputInit()
{
#ifdef USE_DIAL_ROTARY_ENCODER
	rotaryEncoder_init();
#endif
#ifdef USE_KEYS
	keys_init();
#endif
#ifdef USE_POSITION_ADC
	ADC_OPEN_PRM prm;
	prm.mode = ADC_MODE_AUTO;
	prm.adj = ADC_ADJ_LEFT;
	//prm.mux = ADC_MUX_ADC6 | ADC_MUX_ADC7;
	prm.mux = ADC_MUX_ADC6;
	prm.prescale = ADC_PRESCALE_DIV128;
	adcOpen(&prm);
	adcStart();
#endif
#ifdef USE_ACCEL_ADXL345
	accelADXL345_init();
#endif
	setIntervalTimer(updateInputAll, INPUT_CHECK_INTERVAL_MS);
}

RET setInputDialCallback(void (*func)(int8_t))
{
	if(s_dialCallback != 0) return RET_ERR_REGISTER;
	s_dialCallback = func;
	return RET_OK;
}

RET clearInputDialCallback(void (*func)(int8_t))
{
	if(s_dialCallback != func) return RET_ERR_PRM;
	s_dialCallback = 0;
	return RET_OK;
}

RET setInputKeysCallback(void (*func)(uint8_t))
{
	if(s_keysCallback != 0) return RET_ERR_REGISTER;
	s_keysCallback = func;
	return RET_OK;
}

RET clearInputKeysCallback(void (*func)(uint8_t))
{
	if(s_keysCallback != func) return RET_ERR_PRM;
	s_keysCallback = 0;
	return RET_OK;
}

RET setInputPositionCallback(void (*func)(uint8_t))
{
	if(s_positionCallback != 0) return RET_ERR_REGISTER;
	s_positionCallback = func;
	return RET_OK;
}

RET clearInputPositionCallback(void (*func)(uint8_t))
{
	if(s_positionCallback != func) return RET_ERR_PRM;
	s_positionCallback = 0;
	return RET_OK;
}

RET setInputTapCallback(void (*func)(uint8_t))
{
	if(s_tapCallback != 0) return RET_ERR_REGISTER;
	s_tapCallback = func;
	return RET_OK;
}

RET clearInputTapCallback(void (*func)(uint8_t))
{
	if(s_tapCallback != func) return RET_ERR_PRM;
	s_tapCallback = 0;
	return RET_OK;
}

inline int8_t getInputAccelX()
{
#ifdef USE_ACCEL_ADXL345
	return accelADXL345_getAccelX();
#else
	return 0;
#endif
}

inline int8_t getInputAccelY()
{
#ifdef USE_ACCEL_ADXL345
	return accelADXL345_getAccelY();
#else
	return 0;
#endif
}

inline int8_t getInputAccelZ()
{
#ifdef USE_ACCEL_ADXL345
	return accelADXL345_getAccelZ();
#else
	return 0;
#endif
}
/*** Internal Function Definitions ***/ 
static void updateInputAll()
{
	/* update dial input */
	int8_t rotSpeedTick = checkDial();
	static int8_t s_previousRotSpeedTick;
	if(rotSpeedTick == 0 && s_previousRotSpeedTick != 0) {
		if(s_dialCallback != 0) s_dialCallback(0);
	}
	if(rotSpeedTick != 0) {
		if(s_dialCallback != 0) s_dialCallback(rotSpeedTick);
	}
	s_previousRotSpeedTick = rotSpeedTick;

	
	/* update key input */
	uint8_t keys = checkKeys();
	static uint8_t s_previousKeys;
	if(keys != s_previousKeys) {
		s_previousKeys = keys;
		if(s_keysCallback != 0) s_keysCallback(keys);
	}
	
	/* update position input */
	uint8_t position = checkPosition();
	static uint8_t s_previousPosition;
	if(position != s_previousPosition) {
		s_previousPosition = position;
		if(s_positionCallback != 0) s_positionCallback(position);
	}
	
	/* update shake input */
	uint8_t tapNum = checkTap();
	static uint8_t s_previousTapNum;
	if(tapNum != s_previousTapNum) {
		s_previousTapNum = tapNum;
		if(s_tapCallback != 0) s_tapCallback(tapNum);
	}
}

static int8_t checkDial(uint16_t interval)
{
	/* return tick/sec */
	int8_t rotation = 0;
#ifdef USE_DIAL_ROTARY_ENCODER
	rotation = getResetRotaryEncoderRotationTick();
#endif
	//int8_t rotSpeedTick = rotation * (1000/interval) / 4;	// 1 tick = 4 cycle
	int8_t rotSpeedTick = (rotation * 1000 >> 2) >> INPUT_CHECK_INTERVAL_MS_EXP;
	return rotSpeedTick;

}

static uint8_t checkKeys()
{
	uint8_t keys = 0;
#ifdef USE_KEYS
	keys = getKeys();
#endif
	return keys;
}

static uint8_t checkPosition()
{
	uint8_t position = 0;
#ifdef USE_POSITION_ADC
	position = getAdc(ADC_MUX_ADC6);
#endif
	return position >> (8 - POSITION_MAX_VAL_EXP);
}

static uint8_t checkTap()
{
	uint8_t tapNum = 0;
#ifdef USE_ACCEL_ADXL345
	tapNum = (uint8_t)accelADXL345_getResetTapStatus();
#endif
	return tapNum;
}
