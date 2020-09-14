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
			if(!pSimHandler->writeCommand("AT+SAPBR=1,1")){
				if(pSimHandler->isAnwserReady()){
					if(pSimHandler->isAnwserCodeEqual(Anwser_codes::ERROR)){
						currentState = checkingConnection;
					}else{
						currentState = open;
					}
				}
			}
			// if(pSimHandler->writeCommandAndCheckAnwser()){
				// currentState = open;
			// }
		break;
		case checkingConnection:
			if(pSimHandler->writeCommandAndCheckAnwser("AT+SAPBR=2,1")){
				currentState = parseQueryString();
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
			startHandler.close();
		}
	}
	return currentState;
}

/**
	Message looks like:
		+SAPBR:cid,status,ip
		cid - index of barier
		status:
			0 - connecting
			1 - connected
			2 - closing
			3 - closed
	
	
*/

GPRS_state GPRS::parseQueryString(){
	String mes = pSimHandler->getResultMessage();
	mes.remove(0, 7);
	char c = mes[0];
	int status = String(c).toInt();
	switch (status){
		case 0: case 1:
			return open;
		case 2: case 3:
			return closed;
	}
	return closed;
}
