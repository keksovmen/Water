#pragma once


const static char* DIGIT_SUCCESS = "0\r\n";
const static char* DIGIT_COMPLEX_SUCCESS = "\r\n0\r\n";

const static char* DIGIT_ERROR = "4\r\n";
const static char* DIGIT_COMPLEX_ERROR = "\r\n4\r\n";

const static char* TEXT_SUCCESS = "\r\nOK\r\n";
const static char* TEXT_ERROR = "\r\nERROR\r\n";

const static char* END_LINE = "\r\n";


const static char* RING_STRING = "\r\nRING\r\n";
const static char* NO_CARRIER_STRING = "\r\nNO CARRIER\r\n";
const static char* UNDER_VOLTAGE_WARNING = "\r\nUNDER-VOLTAGE WARNNING\r\n";
const static char* OVER_VOLTAGE_WARNING = "\r\nOVER-VOLTAGE WARNNING\r\n";


const static char* HTTPREAD_ANWSER = "\r\n+HTTPREAD: ";


const static char* TCP_CONNECTE_OK = "\r\nCONNECT OK\r\n";
const static char* TCP_ALREADY_CONNECTE = "\r\nALREADY CONNECT\r\n";
const static char* TCP_CONNECT_FAIL = "\r\nCONNECT FAIL\r\n";
const static char* TCP_SHUT_OK = "\r\nSHUT OK\r\n";
const static char* TCP_CONNECTION_CLOSED = "\r\nCLOSED\r\n";
const static char* TCP_INCOMING_MESSAGE = "\r\n+CIPRXGET: 1\r\n";
const static char* TCP_DATA_LENGTH_ANWSER = "\r\n+CIPRXGET: 4,";
const static char* TCP_READ_ANWSER = "\r\n+CIPRXGET: 2,";
const static char* TCP_SEND_OK = "\r\nSEND OK\r\n";
const static char* TCP_SEND_FAIL = "\r\nSEND FAIL\r\n";


const static char* PDP_DEACT = "\r\n+PDP: DEACT\r\n";
const static char* SAPBR_DEACT = "\r\n+SAPBR 1: DEACT\r\n";

