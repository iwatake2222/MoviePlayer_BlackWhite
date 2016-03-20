/*
 * taskManager.h
 *
 * Created: 2016-03-13 10:55:59 PM
 *  Author: take-iwiw
 */ 


#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

/***
 * Const values
 ***/
#define MAX_TASK 10

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/
RET registerInit(void (*func)());
RET registerLoop(void (*func)());
void systemInit();
void systemLoop();
uint8_t getMaxProcessingTIme();
uint8_t getCurrentProcessingTime();

#endif /* TASKMANAGER_H_ */