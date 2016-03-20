/*
 * keys.c
 *
 * Created: 2016-03-09 10:26:01 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include "../myLibraries/myTimer.h"
#include "keysConfig.h"
#include "keys.h"

/*** Internal Const Values ***/
/*** Internal Static Variables ***/
static uint8_t s_fixedKeys;

/*** Internal Function Declarations ***/
static void updateKeys();

/*** External Function Defines ***/
void keys_init()
{
	s_fixedKeys = (1 << KEYS_MAX) - 1;

#if KEYS_MAX > 0
	SET_BIT(KEYS_0_DDR, KEYS_0_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_0_PORT, KEYS_0_BIT);
	#endif
#endif
#if KEYS_MAX > 1
	SET_BIT(KEYS_1_DDR, KEYS_1_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_1_PORT, KEYS_1_BIT);
	#endif
#endif
#if KEYS_MAX > 2
	SET_BIT(KEYS_2_DDR, KEYS_2_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_2_PORT, KEYS_2_BIT);
	#endif
#endif
#if KEYS_MAX > 3
	SET_BIT(KEYS_3_DDR, KEYS_3_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_3_PORT, KEYS_3_BIT);
	#endif
#endif
#if KEYS_MAX > 4
	SET_BIT(KEYS_4_DDR, KEYS_4_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_4_PORT, KEYS_4_BIT);
	#endif
#endif
#if KEYS_MAX > 5
	SET_BIT(KEYS_5_DDR, KEYS_5_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_5_PORT, KEYS_5_BIT);
	#endif
#endif
#if KEYS_MAX > 6
	SET_BIT(KEYS_6_DDR, KEYS_6_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_6_PORT, KEYS_6_BIT);
	#endif
#endif
#if KEYS_MAX > 7
	SET_BIT(KEYS_7_DDR, KEYS_7_BIT);
	#ifdef KEYS_USE_PULLUP
	SET_BIT(KEYS_7_PORT, KEYS_7_BIT);
	#endif
#endif

	setIntervalTimer(updateKeys, KEYS_CHECK_INTERVAL_MS);
}

uint8_t getKeys()
{
	return s_fixedKeys;
}

/*** Internal Function Definitions ***/
static void updateKeys()
{
	uint8_t currentKeys = 0;
	static uint8_t s_previousKeys;

#if KEYS_MAX > 0
	currentKeys |= IS_SET(KEYS_0_PIN, KEYS_0_BIT);
#endif
#if KEYS_MAX > 1
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_1_PIN, KEYS_1_BIT);
#endif
#if KEYS_MAX > 2
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_2_PIN, KEYS_2_BIT);
#endif
#if KEYS_MAX > 3
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_3_PIN, KEYS_3_BIT);
#endif
#if KEYS_MAX > 4
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_4_PIN, KEYS_4_BIT);
#endif
#if KEYS_MAX > 5
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_5_PIN, KEYS_5_BIT);
#endif
#if KEYS_MAX > 6
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_6_PIN, KEYS_6_BIT);
#endif
#if KEYS_MAX > 7
	currentKeys <<= 1;
	currentKeys |= IS_SET(KEYS_7_PIN, KEYS_7_BIT);
#endif
	
	if( currentKeys == s_previousKeys) {
		s_fixedKeys = currentKeys;
	}
	s_previousKeys = currentKeys;
}
