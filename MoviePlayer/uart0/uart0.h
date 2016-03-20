/*
 * uart0.h
 *
 * Created: 2016-03-03 12:08:27 AM
 *  Author: take-iwiw
 */ 


#ifndef UART0_H_
#define UART0_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/
typedef enum {
	UART_OPEN_SPEED_9600    = 0,
	UART_OPEN_SPEED_19200   = 1,
	UART_OPEN_SPEED_38400   = 2,
	UART_OPEN_SPEED_115200  = 3,
	UART_OPEN_SPEED_1M      = 4,
	UART_OPEN_SPEED_2M      = 5,
} UART_OPEN_SPEED;

/***
 * Struct definitions
 ***/
typedef struct {
	UART_OPEN_SPEED speed;
} UART_OPEN_PRM;

/***
 * External APIs
 ***/
RET uart0Open(UART_OPEN_PRM *prm);
RET uart0Close();
void uart0Send( uint8_t data );
uint8_t isUart0Recved();
uint8_t uart0Recv();


#endif /* UART0_H_ */