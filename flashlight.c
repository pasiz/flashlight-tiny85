#include "flashlight.h"

#define ADMUX_VREF	0x21
#define VREF	1100l
#define ADCMAX	1024l

void init_adc(void)
{
	ADCSRA = (1<<ADPS2) | (1<<ADPS1); // prescaler of 64 = 8MHz/64 = 125KHz.
	ADMUX = ADMUX_VREF;	// 0x21 = analogue ref = VCC, input channel = VREF
	ADCSRA |= (1<<ADEN); //  Enable ADC
	_delay_ms(3);	// need a 2ms delay for vref to settle
}

uint16_t vcc(void)
{
	uint16_t result=0;
	int32_t intermediate=0;
	ADCSRA |= (1<<ADSC); // start conversion by writing 1 to ADSC
	while((ADCSRA & (1<<ADSC)) !=0);	// wait until ADSC is clear
	// repeat (datasheet 16.6.2)
	ADCSRA |= (1<<ADSC); // start conversion by writing to ADSC
	while((ADCSRA & (1<<ADSC)) !=0);	// wait until ADSC is clear
	result = ADC;
	// now to convert to vcc. Work in millivolts
	// adc = 1024*vref/vcc. Therefore vcc = 1024*vref/adc
	intermediate = (ADCMAX*VREF) / result;
	result=intermediate & 0xffff;
	return (result);
}

void setup()
{
	init_adc();
}

void loop()
{
	
}
