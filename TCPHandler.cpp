#include "TCPHandler.h"

#include <Arduino.h>
#include "Util.h"


template<int N>
TCPHandler<N>::TCPHandler(
				SimCommandPort& simPort,
				SimResultParser<N>& parser,
				ParameterHandler& parameters
				) :
	refPort(simPort), refParser(parser),
	refParameters(parameters)
{
	
}


template<int N>
bool TCPHandler<N>::isConnected(){
	return state == TCP_STATE::TCP_STATE_CONNECTED;
}


template<int N>
bool TCPHandler<N>::connect(){
	if(isConnected())
		return true;
	
	
	switch(state){
		case TCP_STATE_WRITING_DEFAULTS:
			if(writeDefaults())
				state = TCP_STATE::TCP_STATE_INITIAL;
			break;
		
		case TCP_STATE_INITIAL :
			if(setAPN()){
				state = TCP_STATE::TCP_STATE_IP_START;
			}else{
				reset();
			}
			
			break;
			
		case TCP_STATE_IP_START: 
			if(connectToGPRS()){
				state = TCP_STATE::TCP_STATE_IP_GPRS_ACT;
			}else{
				state = TCP_STATE::TCP_STATE_IP_CONFIG;
			}
			
			break;
			
		case TCP_STATE_IP_CONFIG:
			if(askStatus()){
				updateState();
			}else{
				Serial.println("NO ANWSER FOR STATUS");
			}
			break;
			
		case TCP_STATE_IP_GPRS_ACT: 
			if(getMyIp()){
				state = TCP_STATE::TCP_STATE_IP_STATUS;
				
			}else{
				if(askStatus()){
					updateState();
				}else{
					Serial.println("NO ANWSER FOR STATUS");
				}
			}
			break;
			
		case TCP_STATE_IP_STATUS:
			if(connecToServer()){
				//fix when UnexpectedHandler sets state of greater power
				if(state == TCP_STATE::TCP_STATE_CONNECTED){
					break;
				}
				
				state = TCP_STATE::TCP_STATE_CONNECTING;
			}else{
				if(askStatus()){
					updateState();
				}else{
					Serial.println("NO ANWSER FOR STATUS");
				}
			}
			break;
			
		case TCP_STATE_CONNECTING:
			//just wair until status will be changed
			break;
			
		case TCP_STATE_CONNECTED:
			//congrats you are connected
			break;
			
		case TCP_STATE_CLOSING:
			if(askStatus()){
					updateState();
				}else{
					Serial.println("NO ANWSER FOR STATUS");
				}
			break;
		
		case TCP_STATE_CLOSED:
			//TODO: add AT+CIPSHUT here
			break;
			
		case TCP_STATE_PDP_DEACT:
			
			break;	
	}
	
	return false;
}



template<int N>
void TCPHandler<N>::reset(){
	state = TCP_STATE::TCP_STATE_WRITING_DEFAULTS;
}


template<int N>
void TCPHandler<N>::connectedSuccessfully(){
	state = TCP_STATE::TCP_STATE_CONNECTED;
}


template<int N>
void TCPHandler<N>::connectionFaild(){
	state = TCP_STATE::TCP_STATE_CLOSED;
}


template<int N>
void TCPHandler<N>::updateState(){
	state = refParser.fetchTCPState();
	if(state == TCP_STATE::TCP_STATE_UNDEFINIED){
		Serial.println("UNDEFINED STATE OF TCP!");
		while(1){}
	}
}


template<int N>
bool TCPHandler<N>::writeDefaults(){
	//TODO: try to write APN SETTING HERE
	//TODO: check if OK or ERROR becuase ERROR means you have to AT+CIPSHUT
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_ON);
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
bool TCPHandler<N>::setAPN(){
	refPort.writeCSTT("internet");
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
bool TCPHandler<N>::connectToGPRS(){
	refPort.writeCIICR();
	return readAndExpectSuccess(refPort, refParser, false, 5000);
}


template<int N>
bool TCPHandler<N>::connectToCGATT(){
	if(askCGATTStatus()){
		return true;
	}
	
	refPort.writeCGATT(false, true);
	
	return readAndExpectSuccess(refPort, refParser, false, 7000);
}


template<int N>
bool TCPHandler<N>::getMyIp(){
	refPort.writeGetIpTCP();
	
	ANWSER_CODES code = readAndGetCode(refPort, refParser);
	if(code == ANWSER_CODES::ERROR){
		return false;
	}
	
	return true;
}


template<int N>
bool TCPHandler<N>::connecToServer(){
	refPort.writeCIPSTART(
			refParameters.getAddress().getValue(),
			8188
			);
	
	//TODO: Make check for ALREADY CONNECT
	return readAndExpectSuccess(refPort, refParser);
}



template<int N>
int TCPHandler<N>::waitForCGATT(){
	unsigned long deltaTime = timeStartOfCGATT - millis();
	unsigned long tmp = deltaTime / 3000;
	
	static unsigned long count = 1;
	
	if(deltaTime > 68000){
		return -1;
		
	}else if(tmp > 3000 * count){
		count = tmp + 1;
		
		if(askCGATTStatus()){
			count = 1;
			return 1;
		}
	}
	
	return 0;
}

template<int N>
bool TCPHandler<N>::askCGATTStatus(){
	refPort.writeCGATT(true);
	if(!readAndExpectSuccess(refPort, refParser)){
		return false;
	}
	
	return refParser.isAttachedToGPRSServices();
}


template<int N>
bool TCPHandler<N>::askStatus(){
	refPort.writeCIPSTATUS();
	
	return readAndExpectSuccess(refPort, refParser);
}