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

const static char* CONNECTE_OK = "\r\nCONNECT OK\r\n";
const static char* ALREADY_CONNECTE = "\r\nALREADY CONNECT\r\n";
const static char* CONNECT_FAIL = "\r\nCONNECT FAIL\r\n";