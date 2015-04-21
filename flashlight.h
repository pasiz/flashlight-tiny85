#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

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
