#include <Arduino.h>
#include "StartHandler.h"


bool StartHandler::init(SimHandler* pHandler){
	pSimHandler = pHandler;
	setDelay(2000);
	return true;
}

int StartHandler::doActions(unsigned long deltaTime) {
	// Serial.println(currentState);
	switch(currentState){
		case waitingForModule:
			if(checkIfTimePassed(deltaTime)){
				pSimHandler->clearPreviousBuffer();
			}
			if(pSimHandler->writeCommandAndCheckAnwser("AT")){
				currentState = waitingForNetwork;
			}
		break;
		case waitingForNetwork:
			if(checkIfTimePassed(deltaTime)){
				pSimHandler->clearPreviousBuffer();
			}
			if(pSimHandler->writeCommandAndCheckAnwser("AT+CREG?")){
				if(parseNetworkRegistration()){
					currentState = ready;
				}
			}
		break;
		default:
		break;
	}
	
	return currentState;
}

bool StartHandler::isReady() const{
	return currentState == ready;
}

void StartHandler::close(){
	currentState = waitingForModule;
}

/**
	After you send AT+CREG? with AT+CREG=0
	you wull get result as +CREG: n,s
		where:
		n = current setting of representation
			0 - if network changes there won't be a notification
			1 - if network changes there is notification
			2 - same as 1 plus response to this command is additional
				info about location area code, and cell id as hex bytes
		s = current status of network
			0 - not registered, and not even trying to
			1 - registered home network
			2 - trying to find a network
			3 - registr denied
			4 - Unkown
			5 - registered, romaning
*/

bool StartHandler::parseNetworkRegistration(){
	String mes = pSimHandler->getResultMessage();
	Serial.println(mes);
	if(mes[mes.length() - 1] == '1')
		return true;
	
	return false;
}