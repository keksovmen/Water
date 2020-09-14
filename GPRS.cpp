#include "GPRS.h"


bool GPRS::init(SimHandler* pHandler){
	pSimHandler = pHandler;
	startHandler.init(pHandler);
	// setDelay(1000);
	return true;
}

int GPRS::doActions(unsigned long deltaTime){
	if(!startHandler.isReady()){
		startHandler.doActions(deltaTime);
		if(!startHandler.isReady()){
			return waitingForStartHandler;
		}
	}
	switch(currentState){
		case closed:
			//set APN param
			if(pSimHandler->writeCommandAndCheckAnwser("AT+SAPBR=3,1,\"APN\",\"internet\"")){
				currentState = readyToOpen;
			}
		break;
		case readyToOpen:
			//open connection
			if(pSimHandler->writeCommandAndCheckAnwser("AT+SAPBR=1,1")){
				currentState = open;
			}
		break;
		default:
		break;
	}
	
	return currentState;
}

int GPRS::close(){
	if(currentState == open){
		if(pSimHandler->writeCommandAndCheckAnwser("AT+SAPBR=0,1")){
			currentState = closed;
		}
	}
	return currentState;
}
