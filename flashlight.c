#include "flashlight.h"

#ifdef VOLTAGE_DISPLAY
void showVoltage() {
	uint16_t millivolts;
	uint8_t i;
	pwm_out(powerLevel[0]);
	millivolts = vcc();
	pwm_disable();
	_delay_ms(VOLTAGE_DIGIT_DELAY_MS);
	i=millivolts / 1000;
	while(i--){
		pwm_enable();
		_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
		pwm_disable();
		_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
	}
	_delay_ms(VOLTAGE_DIGIT_DELAY_MS);
	i=(millivolts / 100 )%10;
	while(i--){
		pwm_enable();
		_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
		pwm_disable();
		_delay_ms(VOLTAGE_PULSE_WIDTH_MS);
	}
	_delay_ms(VOLTAGE_DIGIT_DELAY_MS);
	pwm_out(powerLevel[powerState]);
	pwm_enable();
}
#endif


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
			#ifdef VOLTAGE_DISPLAY
			case VOLTAGE_PRESS:
				showVoltage();
				inputState = 0;
			break;
			#endif
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
    init_adc();
    adc_enable();                   		// ADC on
    _delay_ms(SHORT_PRESS*10);
    shutDown = 0;
} // sleep

ISR(PCINT0_vect) {

}

void init_adc()
{
	ADMUX = ( 0 << REFS0 ) | ( 12 << MUX0 );
	ADCSRA |= ( 1 << ADEN ); 
}

uint16_t vcc()
{
	init_adc();
	uint8_t i=16;
	uint16_t result=0;
	while(i--)  {
		ADCSRA |= (1<<ADSC); //start conv
		while(bit_is_set(ADCSRA,ADSC));
		//while((ADCSRA & (1<<ADSC)) !=0);	//wait for conversion to be ready
	}
	result = ADCW;
	return 1125300L / result;
}

void setup()
{
	init_adc();
	/*			PWM init			*/ 
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0B1); //fast pwm clear OC0B on compare
	TCCR0B = (1 << CS00);
	DDRB |= (1 << PB1);						//PB1 output for pwm
	PORTB |= (1 << PB3);					//Enable button pullup resistor
	shutDown = 1;
	powerState = 1;
}

void loop()
{
	if(vcc() <= VOLTAGE_CUTOFF_MV)
		shutDown = 1;
	if(!shutDown){
		handleinput();
		pwm_out(powerLevel[powerState]);
		pwm_enable();
	}
	else
		sleep();
}
