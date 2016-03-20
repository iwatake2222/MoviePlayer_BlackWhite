/*
 * myCommon.h
 *
 * Created: 2016-03-02 7:37:55 PM
 *  Author: take-iwiw
 */ 


#ifndef MYCOMMON_H_
#define MYCOMMON_H_

/***
 * Const values
 ***/
#define F_CPU 16000000

/***
 * Macros
 ***/
#define SET_BIT(PORT, BIT) (PORT |=  (1<<BIT))
#define CLR_BIT(PORT, BIT) (PORT &= ~(1<<BIT))
#define SET_VAL(PORT, START_BIT, SIZE, VAL) (PORT = (PORT & ~(((1<<SIZE)-1) << START_BIT)) | (VAL << START_BIT))
#define IS_SET(PORT, BIT) ( (PORT&(1<<BIT)) == (1<<BIT) )

/***
 * Enum definitions
 ***/
typedef enum {
	RET_OK            = 0,
	RET_WAR_NO_RESULT = 1,
	RET_ERR           = -1,
	RET_ERR_PRM       = -2,
	RET_ERR_CONFLICT  = -3,
	RET_ERR_REGISTER  = -4,
} RET;

#endif /* MYCOMMON_H_ */
