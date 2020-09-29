#pragma once

#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "Enums.h"


/**
	@param c must be in range 0...9
*/

int characterToInt(char c);


template<int N>
bool readAndExpectSuccess(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, bool isComplex = false){
	if(!wrapper.readToBuffer()){
		return false;
	}
	
	bool rdy = false;
	
	if(isComplex){
		rdy = parser.isComplexMessageReady(wrapper.getBuffer());
	}else{
		rdy = parser.isSimpleMessageReady(wrapper.getBuffer());
	}
	
	if(!rdy){
		if(!wrapper.readToBuffer()){
			return false;
		}
	}
	
	if(static_cast<ANWSER_CODES>
			(parser.fetchResultCode(wrapper.getBuffer()))
				== ANWSER_CODES::OK){
		return true;
	}
	
	return false;
}