/*
 * spi.c
 *
 * Created: 2016-03-03 11:47:45 PM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../myCommon.h"
#include "spiConfig.h"
#include "spi.h"

/*** Internal Const Values ***/
/*** Internal Static Variables ***/
static uint8_t s_IsUsing = 0;
static volatile uint8_t s_spiRecvBuffer[SOFTWARE_BUFFER_SIZE];
static volatile uint8_t s_spiRecvWrite = 0;
static volatile uint8_t s_spiRecvRead = 0;
static volatile uint8_t s_spiSendBuffer[SOFTWARE_BUFFER_SIZE];
static volatile uint8_t s_spiSendWrite = 0;
static volatile uint8_t s_spiSendRead = 0;
volatile uint8_t s_isSending = 0;	// do not support multi core
/*** Internal Function Declarations ***/
static void spiSendSetRegister();

/*** External Function Defines ***/
RET spiOpen(SPI_OPEN_PRM *prm)
{
	if(s_IsUsing != 0) return RET_ERR_CONFLICT;
	s_IsUsing = 1;
	
	/* io settings */
	SET_BIT(SPI_DDR, SPI_MOSI_BIT);
	CLR_BIT(SPI_DDR, SPI_MISO_BIT);
	SET_BIT(SPI_DDR, SPI_SCK_BIT);
	
	/* spi settings */
	uint8_t isLsbFirst = (prm->order == SPI_OPEN_ORDER_LSB_FIRST) ? 1 : 0;
	uint8_t isMaster = (prm->role == SPI_OPEN_ROLE_MASTER) ? 1 : 0;
	uint8_t mode = prm->mode & 0x03;
	uint8_t div = prm->div & 0x03;
	uint8_t isDouble = (prm->speed == SPI_OPEN_SPEED_X2) ? 1 : 0;
	uint8_t useInterrupt = (prm->blocking == SPI_OPEN_BLOCKING_NO) ? 1 : 0;
	
	if(isMaster){
		// when spi is master, SS should be output to avoid unexpected action
		SPI_DDR |= 1 << SPI_SS_BIT;
	}
	
	uint8_t spcr = (useInterrupt<<SPIE) | (1<<SPE) | (isLsbFirst<<DORD) | (isMaster<<MSTR) | (mode<<CPHA) | div;
	SPCR = spcr;
	SPSR |= (isDouble == 0 ? 0 : 1);
	
	/* control software initialize */
	s_spiRecvWrite = 0;
	s_spiRecvRead = 0;
	s_spiSendWrite = 0;
	s_spiSendRead = 0;
	s_isSending = 0;
	return RET_OK;
}

RET spiClose()
{
	while(s_isSending);
	SPCR = 0;
	s_IsUsing = 0;
	return RET_OK;
}


void spiSendFast(uint8_t data)
{
	while(((s_spiSendWrite + 1) & (SOFTWARE_BUFFER_SIZE-1)) == s_spiSendRead);
	s_spiSendBuffer[s_spiSendWrite++] = data;
	s_spiSendWrite &= (SOFTWARE_BUFFER_SIZE-1);
	if( !s_isSending ){
		s_isSending = 1;	
		spiSendSetRegister();
	}
}


inline void spiSendBlocking(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

uint8_t spiRecv()
{
	uint8_t data;
	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	data = SPDR;
	return data;
}

uint8_t spiSendRecv(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	data = SPDR;
	return data;
}

/*** Internal Function Definitions ***/
static void spiSendSetRegister()
{
	if (s_spiSendWrite != s_spiSendRead){
		SPDR = s_spiSendBuffer[s_spiSendRead++];
		s_spiSendRead &= (SOFTWARE_BUFFER_SIZE-1);		
	} else {
		s_isSending = 0;
	}
}

ISR(SPI_STC_vect)
{
	spiSendSetRegister();	
}
