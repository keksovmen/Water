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

#define ENTRY_BEGINING "#"
#define ENTRY_ENDING "$"



