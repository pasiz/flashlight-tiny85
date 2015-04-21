#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ADMUX_VREF	0x21
#define VREF	1100l
#define ADCMAX	1024l

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define pwm_out(x)	OCR0B = x
#define pwm_on() DDRB |= (1 << PB1)
#define pwm_off() DDRB &= ~(1 << PB1)

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
