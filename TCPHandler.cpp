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
	// if(state != TCP_STATE::TCP_STATE_WRITING_DEFAULTS){
		// tryUpdateState();
	// }
	
	if(isConnected()){
		return true;
	}
	
	
	switch(state){
		case TCP_STATE_WRITING_DEFAULTS:
			if(writeDefaults()){
				state = TCP_STATE::TCP_STATE_INITIAL;
			}else{
				tryUpdateState();
			}
			break;
		
		case TCP_STATE_INITIAL :
			if(setAPN()){
				state = TCP_STATE::TCP_STATE_IP_START;
			}else{
				tryUpdateState();
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
			tryUpdateState();
			break;
			
		case TCP_STATE_IP_GPRS_ACT: 
			if(getMyIp()){
				state = TCP_STATE::TCP_STATE_IP_STATUS;
				
			}else{
				tryUpdateState();
			}
			break;
			
		case TCP_STATE_IP_STATUS:
			if(connecToServer()){
				//fix when UnexpectedHandler sets state of greater power
				// if(state == TCP_STATE::TCP_STATE_CONNECTED){
					// break;
				// }
				
				state = TCP_STATE::TCP_STATE_CONNECTING;
			}else{
				tryUpdateState();
			}
			break;
			
		case TCP_STATE_CONNECTING:
			//just wair until status will be changed
			tryUpdateState();
			break;
			
		case TCP_STATE_CONNECTED:
			//congrats you are connected
			break;
			
		case TCP_STATE_CLOSING:
			tryUpdateState();
			break;
		
		case TCP_STATE_CLOSED:
			state = TCP_STATE::TCP_STATE_IP_STATUS;
			break;
			
		case TCP_STATE_PDP_DEACT:
			if(tryToShutConenction()){
				state = TCP_STATE::TCP_STATE_INITIAL;
			}
			break;
			
		case TCP_STATE_UNDEFINIED:
			Serial.println("UNDEFINED");
			while(1){}
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
void TCPHandler<N>::shutOk(){
	state = TCP_STATE::TCP_STATE_INITIAL;
}


template<int N>
void TCPHandler<N>::closedConnection(){
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
void TCPHandler<N>::incomingMessage(){
	hasMessage = true;
}


template<int N>
void TCPHandler<N>::clearMessage(){
	hasMessage = false;
}


template<int N>
TCPReader<N> TCPHandler<N>::readMessage(FixedBuffer<N> buffer){
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_INFO);
	int length = 0;
	
	if(readAndExpectSuccess(refPort, refParser)){
		length = refParser.parseRxGetLength();
	}else{
		Serial.println("ERRROR NO ANWSER FROM LENGTH");
	}
	
	return TCPReader<N>(refParser, refPort, buffer, length);
}


template<int N>
bool TCPHandler<N>::writeDefaults(){
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_ON);
	
	if(readAndExpectSuccess(refPort, refParser)){
		return true;
	}
	
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_MODE);
	if(readAndExpectSuccess(refPort, refParser)){
		return refParser.fetchRxGetStatus() == 1;
	}
	
	return false;
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
bool TCPHandler<N>::tryToShutConenction(){
	TCP_STATE oldState = state;
	refPort.writeCIPSHUT();
	refPort.readTimeout(10000);
	if(state == oldState){
		return false;
	}
	
	return true;
	// return readAndExpectSuccess(refPort, refParser, false, 7000);
}


template<int N>
bool TCPHandler<N>::askStatus(){
	refPort.writeCIPSTATUS();
	
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
void TCPHandler<N>::tryUpdateState(){
	if(askStatus()){
		updateState();
	// }else{
		// Serial.println("NO ANWSER FOR STATUS");
		// state = TCP_STATE::TCP_STATE_UNDEFINIED;
	}
}