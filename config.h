/*
 * Config file for flashlight
 */

//voltage show functionality
#define VOLTAGE_DISPLAY

//pressure lenghts
#define SHORT_PRESS		10
#define LONG_PRESS		30
#define SHUTDOWN_PRESS	90

#ifdef VOLTAGE_DISPLAY
#define VOLTAGE_PRESS	400
#define VOLTAGE_PULSE_WIDTH_MS	200
#define VOLTAGE_DIGIT_DELAY_MS	400
#endif

//Voltage cutoff in millivolts
#define VOLTAGE_CUTOFF_MV		2700

//power steps definitions
#define POWER_STEPS		5
const uint8_t powerLevel[] = { 5 , 50 , 100 , 200 , 255 };
