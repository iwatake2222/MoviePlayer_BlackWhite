/*
 * i2c.c
 *
 * Created: 2016-03-14 3:14:53 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "i2c.h"

/*** Internal Const Values ***/
#define WRITE 0
#define READ 1

/*** Internal Static Variables ***/
static uint8_t s_isUsing = 0;

/*** Internal Function Declarations ***/
static void i2cStartMaster();
static void i2cStopMaster();
static void i2cSend(uint8_t data);
static uint8_t i2cRecv(uint8_t needAck);

/*** External Function Defines ***/
RET i2cOpen(I2C_OPEN_PRM *prm)
{
	if(s_isUsing != 0) return RET_ERR_CONFLICT;
	s_isUsing = 1;
	TWSR = 0;	// Prescaler = 1
	// TWBR = (CPU/scl-16)/2
	TWBR = ((F_CPU/1000)/prm->sclKHz - 16) / 2;
	return RET_OK;
}

RET i2cClose()
{
	TWCR = 0;
	s_isUsing = 0;	
	return RET_OK;
}

void i2cWriteMaster(uint8_t slaveAddr, uint8_t regAddr, uint16_t dataNum, uint8_t data[])
{
	i2cStartMaster();
	i2cSend( (slaveAddr << 1) | WRITE);
	i2cSend(regAddr);
	for(uint16_t i = 0; i < dataNum; i++) {
		i2cSend(*data);
		data++;
	}
	i2cStopMaster();
	
	//_delay_ms(1);
}

void i2cReadMaster(uint8_t slaveAddr, uint8_t regAddr, int8_t dataNum, uint8_t data[])
{
	i2cStartMaster();
	i2cSend( (slaveAddr << 1) | WRITE);
	i2cSend(regAddr);
	i2cStartMaster();
	i2cSend( (slaveAddr << 1) | READ);
	for(int8_t i = 0; i < dataNum - 1; i++) {
		*data = i2cRecv(1);
		data++;
	}
	*data = i2cRecv(0);
	
	i2cStopMaster();
}

/*** Internal Function Definitions ***/
static void i2cStartMaster()
{
	TWCR = 1 << TWINT | 1 << TWSTA | 1 << TWEN;
	//while(!IS_SET(TWCR, TWINT));
	for(uint8_t escape = 0 ; escape < 100; escape++){
		if(IS_SET(TWCR, TWINT))break;
		_delay_us(1);
	}
	
}

static void i2cStopMaster()
{
	TWCR = 1 << TWINT | 1 << TWSTO | 1 << TWEN;
	//while(!IS_SET(TWCR, TWSTO));
	for(uint8_t escape = 0 ; escape < 100; escape++){
		if(IS_SET(TWCR, TWSTO))break;
		_delay_us(1);
	}
}

static void i2cSend(uint8_t data)
{	TWDR = data;
	TWCR = 1 << TWINT | 1 << TWEN;
	//while(!IS_SET(TWCR, TWINT));
	for(uint8_t escape = 0 ; escape < 100; escape++){
		if(IS_SET(TWCR, TWINT))break;
		_delay_us(1);
	}
}

static uint8_t i2cRecv(uint8_t needAck)
{
	if (needAck) {
		TWCR = 1 << TWINT | 1 << TWEN | 1 << TWEA;
		} else {
		TWCR = 1 << TWINT | 1 << TWEN;
	}

	//while(!IS_SET(TWCR, TWINT));
	for(uint8_t escape = 0 ; escape < 100; escape++){
		if(IS_SET(TWCR, TWINT))break;
		_delay_us(1);
	}	
	return TWDR;
}
