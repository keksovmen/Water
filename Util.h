#pragma once

// #include <Arduino.h>
#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "Enums.h"


/**
	@param c must be in range 0...9
*/

int characterToInt(char c);


template<int N>
bool readAndExpectSuccess(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, bool isComplex = false){
	return readAndGetCode(wrapper, parser) == ANWSER_CODES::OK;
}


template<int N>
ANWSER_CODES readAndGetCode(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, bool isComplex = false){
	if(!wrapper.readToBuffer()){
		return ANWSER_CODES::UNDEFINED;
	}
	
	bool rdy = false;
	
	if(isComplex){
		rdy = parser.isComplexMessageReady();
	}else{
		rdy = parser.isSimpleMessageReady();
	}
	
	if(!rdy){
		if(!wrapper.readToBuffer()){
			return ANWSER_CODES::UNDEFINED;
		}
	}
	
	return static_cast<ANWSER_CODES>(parser.fetchResultCode());
}