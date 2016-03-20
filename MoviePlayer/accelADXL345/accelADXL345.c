/*
 * accelADXL345.c
 *
 * Created: 2016-03-17 9:43:16 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "accelADXL345.h"
#include "accelADXL345Config.h"
#include "../i2c/i2c.h"
#include "../myLibraries/myTimer.h"

/*** Internal Const Values ***/
#define SLAVE_ADDR 0x53

/*** Internal Static Variables ***/
static int8_t s_accelX, s_accelY, s_accelZ;
static ACCEL_ADXL345_TAP s_accelTap;
/*** Internal Function Declarations ***/
static void getAccelInformation();

/*** External Function Defines ***/
void accelADXL345_init()
{
	I2C_OPEN_PRM prm;
	prm.sclKHz = 400;
	if(i2cOpen(&prm) != RET_OK) return;
	uint8_t data[6];
	//i2cReadMaster(SLAVE_ADDR, 0, 1, data);
	
	data[0] = 0x00;
	i2cWriteMaster(SLAVE_ADDR, 0x2d, 1, data);

	// range = +-2g, Left Justify, 10-bit mode(but will ignore tha right 2-bit)
	data[0] = 0x04;
	i2cWriteMaster(SLAVE_ADDR, 0x31, 1, data);
	
	// setings for tap (values are from data sheet)
	data[0] = 0x60;
	i2cWriteMaster(SLAVE_ADDR, 0x1d, 1, data);
	data[0] = 0x20;
	data[1] = 0x20;
	data[2] = 0x80;
	i2cWriteMaster(SLAVE_ADDR, 0x21, 3, data);
	// enable tap detection
	data[0] = 0x07;
	i2cWriteMaster(SLAVE_ADDR, 0x2a, 1, data);
	data[0] = 0x60;
	i2cWriteMaster(SLAVE_ADDR, 0x2e, 1, data);						
			
	// start measurement
	data[0] = 0x08;
	i2cWriteMaster(SLAVE_ADDR, 0x2d, 1, data);
	i2cClose();
	
	setIntervalTimer(getAccelInformation, ACCEL_ADXL345_CHECK_INTERVAL_MS);
}

inline int8_t accelADXL345_getAccelX()
{
	return s_accelX;
}

inline int8_t accelADXL345_getAccelY()
{
	return s_accelY;
}

inline int8_t accelADXL345_getAccelZ()
{
	return s_accelZ;
}

inline ACCEL_ADXL345_TAP accelADXL345_getResetTapStatus()
{
	ACCEL_ADXL345_TAP ret = s_accelTap;
	s_accelTap = ACCEL_ADXL345_TAP_NONE;
	return ret;
}

/*** Internal Function Defines ***/
static void getAccelInformation()
{	
	I2C_OPEN_PRM prm;
	prm.sclKHz = 400;
	if(i2cOpen(&prm) != RET_OK) return;
	
	uint8_t data[6];
	i2cReadMaster(SLAVE_ADDR, 0x30, 1, data);
	
	if(data[0]&0x20) {
		s_accelTap = ACCEL_ADXL345_TAP_DOUBLE;
	} else if(data[0]&0x40) {
		s_accelTap = ACCEL_ADXL345_TAP_SINGLE;	
	} else {
		// clear the status when user read
		//s_accelTap = ACCEL_ADXL345_TAP_NONE;
	}

	
	i2cReadMaster(SLAVE_ADDR, 0x32, 6, data);
	s_accelX = data[1];
	s_accelY = data[3];
	s_accelZ = data[5];
	
	i2cClose();
}
