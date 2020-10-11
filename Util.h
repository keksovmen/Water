#pragma once

// #include <Arduino.h>
#include "BaseReader.h"
#include "SimResultParser.h"
#include "Enums.h"


/**
	@param c must be in range 0...9
*/

int characterToInt(char c);


/**
	Finds length in characters of given number
	
	@param val
	@return length of val
*/

int findLongLength(long val);


/**
	Find length in characters of given float point number
	
	@param val
	@param afterDot amount of digits after dot
	
	@return length of val + afterDot
*/

int findDoubleLength(double val, int afterDot);

/**
	Tries to read from buffer
	and parse result anwser
	
	@param wrapper only for read() method
	@param parser to find if message is full and fetch code
	@param isComplex for what type of message to expect
	
	@return true if result is present and success

*/

template<int N>
bool readAndExpectSuccess(BaseReader& reader, SimResultParser<N>& parser, bool isComplex = false){
	return readAndGetCode(reader, parser) == ANWSER_CODES::OK;
}


/**
	Tries to read from buffer
	and parse result anwser
	
	@param reader only for read() method
	@param parser to find if message is full and fetch code
	@param isComplex for what type of message to expect
	
	@return ANWSER_CODES if success or mine UNDEFINED if failed

*/

template<int N>
ANWSER_CODES readAndGetCode(BaseReader& reader, SimResultParser<N>& parser, bool isComplex = false){
	if(!reader.read()){
		return ANWSER_CODES::UNDEFINED;
	}
	
	bool rdy = false;
	
	if(isComplex){
		rdy = parser.isComplexMessageReady();
	}else{
		rdy = parser.isSimpleMessageReady();
	}
	
	if(!rdy){
		if(!reader.read()){
			return ANWSER_CODES::UNDEFINED;
		}
	}
	
	return static_cast<ANWSER_CODES>(parser.fetchResultCode());
}
