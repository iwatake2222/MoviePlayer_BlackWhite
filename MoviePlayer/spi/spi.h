/*
 * spi.h
 *
 * Created: 2016-03-03 11:42:14 PM
 *  Author: take-iwiw
 */ 

#ifndef SPI_H_
#define SPI_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/
typedef enum {
	SPI_OPEN_ORDER_MSB_FIRST = 0,
	SPI_OPEN_ORDER_LSB_FIRST = 1,
} SPI_OPEN_ORDER;

typedef enum {
	SPI_OPEN_ROLE_MASTER = 0,
	SPI_OPEN_ROLE_SLAVE  = 1,
} SPI_OPEN_ROLE;

typedef enum {
	SPI_OPEN_MODE_0 = 0,
	SPI_OPEN_MODE_1 = 1,
	SPI_OPEN_MODE_2 = 2,
	SPI_OPEN_MODE_3 = 3,
} SPI_OPEN_MODE;

typedef enum {
	SPI_OPEN_DIV_4   = 0,
	SPI_OPEN_DIV_16  = 1,
	SPI_OPEN_DIV_64  = 2,
	SPI_OPEN_DIV_128 = 3,
} SPI_OPEN_DIV;

typedef enum {
	SPI_OPEN_SPEED_X1 = 0,
	SPI_OPEN_SPEED_X2 = 1,
} SPI_OPEN_SPEED;

typedef enum {
	SPI_OPEN_BLOCKING_YES = 0,
	SPI_OPEN_BLOCKING_NO  = 1,
} SPI_OPEN_BLOCKING;

/***
 * Struct definitions
 ***/
/* note: if a target device require other controls such as CS, R/W. Need to use blocking */
typedef struct {
	SPI_OPEN_ORDER    order;
	SPI_OPEN_ROLE     role;
	SPI_OPEN_MODE     mode;
	SPI_OPEN_DIV      div;
	SPI_OPEN_SPEED    speed;
	SPI_OPEN_BLOCKING blocking;	// complete transfer in the function call
} SPI_OPEN_PRM;

/***
 * External APIs
 ***/
RET spiOpen(SPI_OPEN_PRM *prm);
RET spiClose();
void spiSendFast(uint8_t data);
void spiSendBlocking(uint8_t data);
uint8_t spiRecv();
uint8_t spiSendRecv(uint8_t data);

#endif /* SPI_H_ */
