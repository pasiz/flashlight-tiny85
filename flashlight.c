#include "flashlight.h"

void showVoltage() {
	uint16_t voltage;
	uint8_t digits[4];
	uint8_t x,y,i;
	pwm_out(powerLevel[1]);
	voltage = vcc();
	pwm_disable();
	_delay_ms(VOLTAGE_DIGIT_DELAY_MS);
	for(i=3;i>0;i--){
		digits[i] = voltage%10;
		voltage /= 10;
	}
	for(x=0;x<3;x++){
		for(y=0;y<digits[x];y++){
			pwm_enable();
			_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
			pwm_disable();
			_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
		}
		_delay_ms(VOLTAGE_DIGIT_DELAY_MS);
	}
	pwm_out(powerLevel[powerState]);
	pwm_enable();
	
}


void handleinput() {
	uint16_t inputTime=0;
	uint8_t inputState=0;
	while(!(PINB & (1 << PB3)))
	{
		inputTime++;
		switch(inputTime){
			case SHORT_PRESS:
				inputState = 1;
			break;
			case LONG_PRESS:
				inputState = 2;
			break;
			case SHUTDOWN_PRESS:
				pwm_disable();
				_delay_ms(10);
				pwm_enable();
				inputState = 3;
			break;
			case VOLTAGE_PRESS:
				showVoltage();
				inputState = 0;
			break;
		}
		_delay_ms(10);
	}
	if(inputState == 1)
		powerUp();
	if(inputState == 2)
		powerDown();
	if(inputState == 3)
		shutDown = 1;
	
}	//readinput

void sleep() {
	pwm_disable();
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    adc_disable();                   		// ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    adc_enable();                   		// ADC on
    _delay_ms(SHORT_PRESS*10);
	if((PINB & (1 << PB3)))
		shutDown = 0;
	else
		shutDown = 1;
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
	cli();
	init_adc();
	/*			PWM init			*/ 
	TCCR0B |= (1 << CS01) | (1 << CS00);	//PWM prescale
	TCCR0A |= (1 << WGM01) | (1 << WGM00);	//Fast pwm
	TCCR0A |= (1 << COM0B1);				//clear OC0B output on compare, upward counting
	DDRB |= (1 << PB1);						//PB1 output for pwm
	PORTB |= (1 << PB3);					//Enable button pullup resistor
	shutDown = 1;
	powerState = 1;
}

void loop()
{
	if(vcc() <= VOLTAGE_CUTOFF_MV)
		shutDown = 1;
	if(shutDown == 0){
		handleinput();
		pwm_out(powerLevel[powerState]);
	}
	else
		sleep();
}
