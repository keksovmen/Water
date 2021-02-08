#include "SimTools.h"
// #include ""

//TODO: when you don't get the anwser set stat flag to undefinied
ANWSER_CODES SimTools::readAndGetCode(unsigned long maxDelay){
	if(!readTimeout(maxDelay)){
		return ANWSER_CODES::UNDEFINED;
	}
	
	bool rdy = false;
	
	
	//TODO: made 1 method for text only, fuck digits
	// if(isComplex){
		rdy = isComplexMessageReady();
	// }else{
		// rdy = isSimpleMessageReady();
	// }
	
	if(!rdy){
		if(!read()){
			return ANWSER_CODES::UNDEFINED;
		}
	}
	
	return static_cast<ANWSER_CODES>(fetchResultCode());
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