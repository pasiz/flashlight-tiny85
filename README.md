# flashlight-tiny85

Flashlight code for attiny85 processor

now you can piggybag existing pwm controlled flashlight driver or make your own

To tune parameters, edit config.h

To flash, edit programmer to Makefile and "make flash"


###Linux install toolchain (ubuntu)

sudo apt-get install gcc-avr avr-libc avrdude make

###Functions

Short press (defined in config.h --> SHORT_PRESS by milliseconds / 10) raises power
Longer press (defined in config.h --> LONG_PRESS by milliseconds / 10) lowers power
Long press (defined in config.h --> SHUTDOWN_PRESS by milliseconds / 10) make shutdown
Really long press (defined in config.h --> VOLTAGE_PRESS by milliseconds / 10) shows the voltage by flashing

undefine VOLTAGE_DISPLAY if you don't want voltage display on flashlight


VOLTAGE_CUTOFF_MV is for liPo protection
POWER_STEPS is the count of power steps, defined in const uint8_t PowerLevel[], pwm value ranging 0-255
