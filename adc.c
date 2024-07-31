#include "adc.h"

static volatile uint16_t lectura=0;

uint8_t adc_flag=0;

void ADC_Init(){
	//Habilita ADC
	ADCSRA |= 1<<ADEN;
	//Clock prescaler en 128 -> sample frequency 125kHz
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;
	//Establece Vcc como referencia 
	ADMUX |= 1<<REFS0;
	//Establece channel 3 (ADC3)
	ADMUX |= 1<<MUX1 | 1<<MUX0;
	//Deshabilita digital input oen el pin ADC3
	DIDR0 |= 1<<ADC3D;
	
	//Habilita las interupciones
	ADCSRA |= 1<<ADIE;
	
}

void ADC_Read(){
	ADCSRA |= 1<< ADSC; //start convertion
}

uint8_t ADC_GetFlag(){
	return adc_flag;
}

void ADC_ClearFlag(){
	adc_flag=0;
}

uint16_t ADC_GetBrillo(){
	return lectura;
}

/*Conversion complete ISR*/
ISR(ADC_vect){
	uint8_t adcl = 0;
	uint8_t adch = 0;
	
	adcl = ADCL;
	adch = ADCH;
	
	lectura = (adch<<8 | adcl);
	
	adc_flag=1;
}