#include "flashlight.h"

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT0_vect) {
	
}

void init_adc()
{
	ADCSRA = (1<<ADPS2) | (1<<ADPS1); //125KHz.
	ADMUX = ADMUX_VREF;
	ADCSRA |= (1<<ADEN); 
	_delay_ms(3);
}

uint16_t vcc()
{
	uint16_t result=0;
	int32_t intermediate=0;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)) !=0);
	ADCSRA |= (1<<ADSC); //start conv
	while((ADCSRA & (1<<ADSC)) !=0);	//wait for conversion to be ready
	result = ADC;
	//vcc = 1024*vref/adc
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
