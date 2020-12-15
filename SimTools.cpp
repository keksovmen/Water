#include "SimTools.h"
// #include ""

//TODO: when you don't get the anwser set stat flag to undefinied
ANWSER_CODES SimTools::readAndGetCode(unsigned long maxDelay){
	if(!simPort.readTimeout(maxDelay)){
		return ANWSER_CODES::UNDEFINED;
	}
	
	bool rdy = false;
	
	
	//TODO: made 1 method for text only, fuck digits
	// if(isComplex){
		rdy = parser.isComplexMessageReady();
	// }else{
		// rdy = parser.isSimpleMessageReady();
	// }
	
	if(!rdy){
		if(!simPort.read()){
			return ANWSER_CODES::UNDEFINED;
		}
	}
	
	return static_cast<ANWSER_CODES>(parser.fetchResultCode());
}


bool SimTools::readAndExpectSuccess(unsigned long maxDelay){
	return readAndGetCode(maxDelay) == OK;
}




ANWSER_CODES SimToolsProxy::readAndGetCode(unsigned long maxDelay){
	ANWSER_CODES code = SimTools::readAndGetCode(maxDelay);
	if(code == UNDEFINED){
		state.encounterUndefinied();
	}
	
	return code;
}