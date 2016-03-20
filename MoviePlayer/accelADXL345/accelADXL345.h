/*
 * accelADXL345.h
 *
 * Created: 2016-03-17 9:43:23 AM
 *  Author: take-iwiw
 */ 


#ifndef ACCELADXL345_H_
#define ACCELADXL345_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/
typedef enum {
	ACCEL_ADXL345_TAP_NONE   = 0,
	ACCEL_ADXL345_TAP_SINGLE = 1,
	ACCEL_ADXL345_TAP_DOUBLE = 2,
} ACCEL_ADXL345_TAP;

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/
void accelADXL345_init();
int8_t accelADXL345_getAccelX();
int8_t accelADXL345_getAccelY();
int8_t accelADXL345_getAccelZ();
ACCEL_ADXL345_TAP accelADXL345_getResetTapStatus();



#endif /* ACCELADXL345_H_ */