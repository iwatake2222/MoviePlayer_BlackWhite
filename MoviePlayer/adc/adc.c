/*
 * adc.c
 *
 * Created: 2016-03-14 12:45:14 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../myCommon.h"
#include "adc.h"

/*** Internal Const Values ***/
#define FLAG_SINGLE 66
#define FLAG_AUTO 77
#define ADC_NUM 8
#define IS_USING 1
#define IS_NOT_USING 0

/*** Internal Static Variables ***/
static uint8_t s_isUsing = 0;
static uint8_t s_adc[ADC_NUM];
static uint8_t s_convertMux[ADC_NUM+1];
static uint8_t s_convertMuxIndex;

/*** Internal Function Declarations ***/


/*** External Function Definitions ***/
RET adcOpen(ADC_OPEN_PRM *prm)
{
	if(s_isUsing == IS_USING) return RET_ERR_CONFLICT;
	s_isUsing = IS_USING;
	
	ADMUX = ADC_REF_AVCC << REFS0 | prm->adj << ADLAR;
	
	uint8_t j = 0;
	for(uint8_t i = 0; i < ADC_NUM+1; i++) {
		 if( (prm->mux & 0x01) != 0) {
			 s_convertMux[j++] = i;
		 }
		 prm->mux >>= 1;
	}
	if(prm->mode == ADC_MODE_AUTO){
		s_convertMux[j] = FLAG_AUTO;
	} else {
		s_convertMux[j] = FLAG_SINGLE;
	}
	
	s_convertMuxIndex = 0;
	ADMUX &= ~0x0F;
	ADMUX |= s_convertMux[s_convertMuxIndex];
	
	ADCSRA = 1 << ADEN | 0 << ADATE | 1 << ADIE;
	ADCSRA |= prm->prescale;	
	
	return RET_OK;
}

RET adcClose()
{
	ADCSRA &= ~(1 << ADEN);
	s_isUsing = IS_NOT_USING;
	return RET_OK;
}

void adcStart()
{
	ADCSRA |= 1 << ADSC;
}

void adcStop()
{
	for(uint8_t i = 0; i < ADC_NUM+1; i++) {
		s_convertMux[i] = FLAG_SINGLE;
	}
}

uint8_t getAdc(ADC_MUX mux)
{
	switch(mux){
		case ADC_MUX_ADC0: return s_adc[0];
		case ADC_MUX_ADC1: return s_adc[1];
		case ADC_MUX_ADC2: return s_adc[2];
		case ADC_MUX_ADC3: return s_adc[3];
		case ADC_MUX_ADC4: return s_adc[4];
		case ADC_MUX_ADC5: return s_adc[5];
		case ADC_MUX_ADC6: return s_adc[6];
		case ADC_MUX_ADC7: return s_adc[7];
		default: return 0;
	}
}

/*** Internal Function Definitions ***/ 
ISR(ADC_vect)
{
	s_adc[s_convertMux[s_convertMuxIndex]] = ADCH;
	s_convertMuxIndex++;
	
	/* when index reaches the end of arrays, check if next conversion is needed */
	if(s_convertMux[s_convertMuxIndex] == FLAG_SINGLE){
		s_convertMuxIndex = 0;
		return;
	}
	if(s_convertMux[s_convertMuxIndex] == FLAG_AUTO){
		s_convertMuxIndex = 0;
		/* do the next turn of conversion from the beginning of array */
	}
	
	/* when index didn't reach the end of array, do the next conversion */
	ADMUX &= ~0x0F;
	ADMUX |= s_convertMux[s_convertMuxIndex];
	adcStart();
}
