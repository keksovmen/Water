#pragma once

#include "Constants.h"

// const static char* DIGIT_SUCCESS = "0\r\n";
// const static char* DIGIT_COMPLEX_SUCCESS = "\r\n0\r\n";

// const static char* DIGIT_ERROR = "4\r\n";
// const static char* DIGIT_COMPLEX_ERROR = "\r\n4\r\n";

// const static char* TEXT_SUCCESS = "\r\nOK\r\n";
// const static char* TEXT_ERROR = "\r\nERROR\r\n";
// const static char* TEXT_CME_ERROR = "+CME ERROR: ";
// const static char* TEXT_RDY = "\r\nRDY\r\n";

// const static char* END_LINE = "\r\n";


// const static char* RING_STRING = "\r\nRING\r\n";
// const static char* NO_CARRIER_STRING = "\r\nNO CARRIER\r\n";
// const static char* UNDER_VOLTAGE_WARNING = "\r\nUNDER-VOLTAGE WARNNING\r\n";
// const static char* OVER_VOLTAGE_WARNING = "\r\nOVER-VOLTAGE WARNNING\r\n";


// const static char* HTTPREAD_ANWSER = "\r\n+HTTPREAD: ";


// const static char* TCP_CONNECTE_OK = "\r\nCONNECT OK\r\n";
// const static char* TCP_ALREADY_CONNECTE = "\r\nALREADY CONNECT\r\n";
// const static char* TCP_CONNECT_FAIL = "\r\nCONNECT FAIL\r\n";
// const static char* TCP_SHUT_OK = "\r\nSHUT OK\r\n";
// const static char* TCP_CONNECTION_CLOSED = "\r\nCLOSED\r\n";
// const static char* TCP_INCOMING_MESSAGE = "\r\n+CIPRXGET: 1\r\n";
// const static char* TCP_DATA_LENGTH_ANWSER = "\r\n+CIPRXGET: 4,";
// const static char* TCP_READ_ANWSER = "\r\n+CIPRXGET: 2,";
// const static char* TCP_SEND_OK = "\r\nSEND OK\r\n";
// const static char* TCP_SEND_FAIL = "\r\nSEND FAIL\r\n";


// const static char* PDP_DEACT = "\r\n+PDP: DEACT\r\n";
// const static char* SAPBR_DEACT = "\r\n+SAPBR 1: DEACT\r\n";



#define DIGIT_SUCCESS  "0\r\n"
#define DIGIT_COMPLEX_SUCCESS  "\r\n0\r\n"

#define DIGIT_ERROR  "4\r\n"
#define DIGIT_COMPLEX_ERROR  "\r\n4\r\n"

#define TEXT_SUCCESS  "\r\nOK\r\n"
#define TEXT_ERROR  "\r\nERROR\r\n"
#define TEXT_CME_ERROR  "+CME ERROR: "
#define TEXT_RDY  "\r\nRDY\r\n"

#define END_LINE  "\r\n"


#define RING_STRING  "\r\nRING\r\n"
#define NO_CARRIER_STRING  "\r\nNO CARRIER\r\n"
#define UNDER_VOLTAGE_WARNING  "\r\nUNDER-VOLTAGE WARNNING\r\n"
#define OVER_VOLTAGE_WARNING  "\r\nOVER-VOLTAGE WARNNING\r\n"





#define TCP_CONNECTE_OK  "\r\nCONNECT OK\r\n"
#define TCP_ALREADY_CONNECTE  "\r\nALREADY CONNECT\r\n"
#define TCP_CONNECT_FAIL  "\r\nCONNECT FAIL\r\n"
#define TCP_SHUT_OK  "\r\nSHUT OK\r\n"
#define TCP_CONNECTION_CLOSED  "\r\nCLOSED\r\n"

#ifdef SIM800
	#define TCP_INCOMING_MESSAGE  "\r\n+CIPRXGET: 1\r\n"
	#define TCP_DATA_LENGTH_ANWSER  "\r\n+CIPRXGET: 4,"
	#define TCP_READ_ANWSER  "\r\n+CIPRXGET: 2,"
#else
	#define TCP_INCOMING_MESSAGE  "\r\n+CIPRXGET:1\r\n"
	#define TCP_DATA_LENGTH_ANWSER  "\r\n+CIPRXGET:4,"
	#define TCP_READ_ANWSER  "\r\n+CIPRXGET:2,"
#endif

#define TCP_SEND_OK  "\r\nSEND OK\r\n"
#define TCP_SEND_FAIL  "\r\nSEND FAIL\r\n"

#ifdef SIM800
	#define HTTP_HTTPACTION "\r\n+HTTPACTION: "
	#define HTTP_HTTPREAD_ANWSER  "\r\n+HTTPREAD: "
#else
	#define HTTP_HTTPACTION "\r\n+HTTPACTION:"
	#define HTTP_HTTPREAD_ANWSER  "\r\n+HTTPREAD:"
#endif

#define PDP_DEACT  "\r\n+PDP: DEACT\r\n"
#define SAPBR_DEACT  "\r\n+SAPBR 1: DEACT\r\n"
