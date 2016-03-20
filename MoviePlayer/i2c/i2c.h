/*
 * i2c.h
 *
 * Created: 2016-03-14 7:06:28 AM
 *  Author: take-iwiw
 */ 


#ifndef I2C_H_
#define I2C_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/
typedef struct {
	uint16_t sclKHz;
} I2C_OPEN_PRM;

/***
 * External APIs
 ***/
RET i2cOpen(I2C_OPEN_PRM *prm);
RET i2cClose();
void i2cWriteMaster(uint8_t slaveAddr, uint8_t regAddr, uint16_t dataNum, uint8_t data[]);
void i2cReadMaster(uint8_t slaveAddr, uint8_t regAddr, int8_t dataNum, uint8_t data[]);

#endif /* I2C_H_ */