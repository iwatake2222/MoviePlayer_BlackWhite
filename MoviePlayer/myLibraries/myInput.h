/*
 * myInput.h
 *
 * Created: 2016-03-09 4:56:36 PM
 *  Author: take-iwiw
 */ 


#ifndef MYINPUT_H_
#define MYINPUT_H_

/***
 * Const Value definitions
 ***/

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/
void inputInit();
RET setInputDialCallback(void (*func)(int8_t));
RET clearInputDialCallback(void (*func)(int8_t));
RET setInputKeysCallback(void (*func)(uint8_t));
RET clearInputKeysCallback(void (*func)(uint8_t));
RET setInputPositionCallback(void (*func)(uint8_t));
RET clearInputPositionCallback(void (*func)(uint8_t));
RET setInputTapCallback(void (*func)(uint8_t));
RET clearInputTapCallback(void (*func)(uint8_t));
int8_t getInputAccelX();
int8_t getInputAccelY();
int8_t getInputAccelZ();

#endif /* MYINPUT_H_ */