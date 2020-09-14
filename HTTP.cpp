#include "HTTP.h"


bool HTTP::init(SimHandler* pHandler){
	gprs.init(pHandler);
	pSimHandler = pHandler;
	// setDelay(100);
	return true;
}

//main purpose is to send post data to server
int HTTP::doActions(unsigned long deltaTime){
	if(dataToSend.length() == 0){
		// currentState = idle;
		return idle;
	}
	
	//handle GPRS opening
	if(!isGPRS_Open(deltaTime)){
		return waitingForGPRS;
	}
	
	if(currentState == idle){
		currentState = readyToWork;
	}
	
	// currentState = isGPRS_Open(deltaTime) ? readyToWork : waitingForGPRS;
	
	switch(currentState){
		case readyToWork:
			//init session
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPINIT")){
				currentState = sessionInitiated;
			}
		break;
		case sessionInitiated:
			//set URL
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPPARA=\"URL\",\"http://95.30.225.225/Send.php\"")){
				currentState = URLsetted;
			}
		break;
		case URLsetted:
			//set CONTENT
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"")){
				currentState = ContentSetted;
			}
		break;
		case ContentSetted:
			//initiate start data input
			//case when you get DOWNLOAD insted of code OK = 0
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPDATA=" + getPostDataString())){
				currentState = readyToFeedData;
			}
		break;
		case readyToFeedData:
			//put Data into
			if(pSimHandler->writeCommandAndCheckAnwser(dataToSend)){
				currentState = dataSetted;
			}
		break;
		case dataSetted:
			//send Data
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPACTION=1")){
				currentState = dataSended;
			}
		break;
		case dataSended:			
			if(pSimHandler->isAnwserReady() && 
				pSimHandler->getResultCode() == Anwser_codes::OK){
					currentState = responceHeaderReaded;
					//handle incoming, such as data length of incoming data
			}
		break;
		case responceHeaderReaded:
			//read data hear
			currentState = dataRead;
		break;
		case dataRead:
			//close GPRS
			if(pSimHandler->writeCommandAndCheckAnwser("AT+HTTPTERM")){
				currentState = closingGPRS;
			}
		break;
		case closingGPRS:
			if(gprs.close() == GPRS_state::closed){
				currentState = idle;
				dataToSend = "";
			}
		break;
		default:
		break;
	}
	
	return currentState;
}

void HTTP::setDataToSend(const String& data){
	dataToSend = data;
}

bool HTTP::isGPRS_Open(unsigned long deltaTime){
	if(gprs.getState() == GPRS_state::open)
		return true;
	if(gprs.doActions(deltaTime) == GPRS_state::open)
		return true;
	
	return false;
}

String HTTP::getPostDataString(){
	return String(dataToSend.length()) + "," + String("30000");
}