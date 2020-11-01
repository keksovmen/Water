#include "Util.h"

#include <Arduino.h>


int characterToInt(char c){
	return c - '0';
}


int findLongLength(long val){
	long delimeter = 1000000000;
	bool started = false;
	int count = 1;

	if(val < 0){
		val = abs(val);
		count++;	//minus sign
	}
	
	while(delimeter > 1){
		if ((val / delimeter) > 0){
			started = true;
			val -= delimeter;
		}
		
		if(started){
			count++;
		}
		
		delimeter /= 10;
	}
	
	return count;
}


int findDoubleLength(double val, int afterDot){
	return findLongLength(val) + afterDot + 1;	//1 for dot character
}

bool readAndExpectSuccess(BaseReader& reader, SimResultParser& parser, bool isComplex, int timeout){
	return readAndGetCode(reader, parser, isComplex, timeout) == ANWSER_CODES::OK;
}


ANWSER_CODES readAndGetCode(BaseReader& reader, SimResultParser& parser, bool isComplex, int timeout){
	if(!reader.readTimeout(timeout)){
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