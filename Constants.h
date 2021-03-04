#pragma once

//undefine for SIM900 module
// #define SIM800

//in chars
#define FIXED_BUFFER_SIZE 128

//in millis
#ifndef SIM800
	#define WRAPPER_MIN_DELAY 50
#else
	#define WRAPPER_MIN_DELAY 25
#endif


#define SIZE_OF_PARAMETER_WRITER 10

//Use in string methods char* not where just char e.g. strstr vs strchr
#define ENTRY_BEGINING "#"
#define ENTRY_ENDING "$"

//determine how long can wait for anwser from sim modeule in millis
#define LONG_WAIT 500

#define START_EEPROM_ADDRESS 0

