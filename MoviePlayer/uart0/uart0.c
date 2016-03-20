/*
 * uart0.c
 *
 * Created: 2016-03-03 12:08:37 AM
 *  Author: take-iwiw
 *    Note: Asynchronous Normal mode fixed
 *          8bit, 2-bit stop bit, no parity
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../myCommon.h"
#include "uart0Config.h"
#include "uart0.h"

/*** Internal Const Values ***/

/*** Internal Static Variables ***/
static volatile uint8_t s_usartRecvBuffer[SOFTWARE_BUFFER_SIZE];
static volatile uint8_t s_usartRecvWrite = 0;
static volatile uint8_t s_usartRecvRead = 0;
static volatile uint8_t s_usartSendBuffer[SOFTWARE_BUFFER_SIZE];
static volatile uint8_t s_usartSendWrite = 0;
static volatile uint8_t s_usartSendRead = 0;
static uint8_t s_isUsing = 0;
/*** Internal Function Declarations ***/
static void uart0SendSetRegister();

/*** External Function Defines ***/
RET uart0Open(UART_OPEN_PRM *prm)
{
	if(s_isUsing != 0) return RET_ERR_CONFLICT;
	s_isUsing = 1;
	/* Set Baud Rate (calc as using double speed) */
	uint16_t ubrr = 0;
	switch(prm->speed){
		default:
		case UART_OPEN_SPEED_9600: ubrr = F_CPU/8/9600 - 1; break;
		case UART_OPEN_SPEED_19200: ubrr = F_CPU/8/19200 - 1; break;
		case UART_OPEN_SPEED_38400: ubrr = F_CPU/8/38400 - 1; break;
		case UART_OPEN_SPEED_115200: ubrr = F_CPU/8/115200 - 1; break;
		case UART_OPEN_SPEED_1M: ubrr = F_CPU/8/1000000 - 1; break;
		case UART_OPEN_SPEED_2M: ubrr = F_CPU/8/2000000 - 1; break;
	} 	
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)(ubrr&0xff);
	UCSR0A |= (1<<U2X0);	// enable double speed

	/* Enable Tx, RX, Tx INT, Rx INT */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);

	/* Asynchronous, 2 stop bits */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
	return RET_OK;
}

RET uart0Close()
{
	s_isUsing = 0;
	return RET_OK;
}

void uart0Send( uint8_t data )
{
	while(((s_usartSendWrite + 1) & (SOFTWARE_BUFFER_SIZE-1)) == s_usartSendRead);
	s_usartSendBuffer[s_usartSendWrite++] = data;
	s_usartSendWrite &= (SOFTWARE_BUFFER_SIZE-1);
	if ( IS_SET(UCSR0A, UDRE0) ){
		uart0SendSetRegister();
	}
}

uint8_t isUart0Recved()
{
	return (s_usartRecvWrite != s_usartRecvRead);
}

uint8_t uart0Recv()
{
	while(s_usartRecvWrite == s_usartRecvRead);
	uint8_t data = s_usartRecvBuffer[s_usartRecvRead++];
	s_usartRecvRead &= (SOFTWARE_BUFFER_SIZE-1);
	return data;
}


/*** Internal Function Definitions ***/
static void uart0SendSetRegister()
{
	if (s_usartSendWrite != s_usartSendRead){
		UDR0 = s_usartSendBuffer[s_usartSendRead++];
		s_usartSendRead &= (SOFTWARE_BUFFER_SIZE-1);
	}
}

ISR(USART_TX_vect)
{
	uart0SendSetRegister();
}


ISR(USART_RX_vect)
{
	/* do not check buffer over flow */
	s_usartRecvBuffer[s_usartRecvWrite++] = UDR0;
	s_usartRecvWrite &= (SOFTWARE_BUFFER_SIZE-1);
}


ISR(USART_UDRE_vect)
{

}



