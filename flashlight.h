#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"

#define ADMUX_VREF	0x21
#define VREF	1100l
#define ADCMAX	1024l

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define pwm_out(x)	OCR0B = x
#define pwm_enable() DDRB |= (1 << PB1)
#define pwm_disable() DDRB &= ~(1 << PB1)

uint8_t shutDown;
uint8_t powerState;

#define powerUp() if(powerState < POWER_STEPS-1) powerState++
#define powerDown() if(powerState > 0) powerState--



void sleep(void);
void init_adc(void);
uint16_t vcc(void);
void setup(void);
void loop(void);

int main(void)
{
	setup();
	while(1)
		loop();
}
