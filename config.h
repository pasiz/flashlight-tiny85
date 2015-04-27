/*
 * Config file for flashlight
 */

//voltage show functionality
#define VOLTAGE_DISPLAY

//pressure lenghts
#define SHORT_PRESS		80
#define LONG_PRESS		250
#define SHUTDOWN_PRESS	900

#ifdef VOLTAGE_DISPLAY
#define USE_CURRENT_POWERLEVEL_VBLINK
#define VBLINK_POWERLEVEL	0
#define MEASURE_VOLTAGE_DISCONNECTED
#define VOLTAGE_RAISE_TIME_MS	50

#define VOLTAGE_PRESS	4000
#define VOLTAGE_PULSE_WIDTH_MS	200
#define VOLTAGE_DIGIT_DELAY_MS	400
#endif

//Voltage cutoff in millivolts
#define VOLTAGE_CUTOFF_MV		2800

//Limited powerlevel on almost empty battery (specify in millivolts)
//#define LIMIT_POWERLEVEL_BY_MV	3000
//#define LIMIT_POWERLEVEL_POWERSTATE 3

//power steps definitions
#define POWER_STEPS		5
const uint8_t powerLevel[] = { 1 , 8 , 30 , 100 , 255 };
