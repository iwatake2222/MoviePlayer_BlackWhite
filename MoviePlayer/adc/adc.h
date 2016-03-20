/*
 * adc.h
 *
 * Created: 2016-03-14 12:45:06 AM
 *  Author: take-iwiw
 */ 

#ifndef ADC_H_
#define ADC_H_

/***
 * Enum definitions
 ***/
typedef enum {
	ADC_MODE_SINGLE = 0,
	ADC_MODE_AUTO   = 1,
} ADC_MODE;

typedef enum  {
	ADC_REF_AREF        = 0,
	ADC_REF_AVCC= 1,
	ADC_REF_RESERVED    = 2,
	ADC_REF_INTERNAL1_1 = 3,
} ADC_REF;

typedef enum {
	ADC_ADJ_RIGHT = 0,
	ADC_ADJ_LEFT  = 1,
} ADC_ADJ;

typedef enum {
	ADC_MUX_ADC0 = 1 << 0,
	ADC_MUX_ADC1 = 1 << 1,
	ADC_MUX_ADC2 = 1 << 2,
	ADC_MUX_ADC3 = 1 << 3,
	ADC_MUX_ADC4 = 1 << 4,
	ADC_MUX_ADC5 = 1 << 5,
	ADC_MUX_ADC6 = 1 << 6,
	ADC_MUX_ADC7 = 1 << 7,
} ADC_MUX;

typedef enum {
	ADC_PRESCALE_DIV2   = 0,
	ADC_PRESCALE_DIV4   = 2,
	ADC_PRESCALE_DIV8   = 3,
	ADC_PRESCALE_DIV16  = 4,
	ADC_PRESCALE_DIV32  = 5,
	ADC_PRESCALE_DIV64  = 6,
	ADC_PRESCALE_DIV128 = 7,
} ADC_PRESCALE;

/***
 * Struct definitions
 ***/
typedef struct {
	ADC_MODE     mode;
	ADC_ADJ      adj;	
	ADC_PRESCALE prescale;
	uint8_t      mux;
} ADC_OPEN_PRM;

/***
 * External APIs
 ***/
RET adcOpen(ADC_OPEN_PRM *prm);
RET adcClose();
void adcStart();
void adcStop();
uint8_t getAdc(ADC_MUX mux);

#endif /* ADC_H_ */