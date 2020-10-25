#include "UnexpectedHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"
#include <Arduino.h>


template<int N>
UnexpectedHandler<N>::UnexpectedHandler(
				FixedBuffer<N>& buffer,
				BaseReader& reader
				) :
	refBuffer(buffer), refReader(reader)
{

}



template<int N>
void UnexpectedHandler<N>::attachTCPHandler(TCPHandler<N>* tcpHandler){
	this->tcpHandler = tcpHandler;
}



template<int N>
bool UnexpectedHandler<N>::read(){
	if(!refReader.read()){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


template<int N>
bool UnexpectedHandler<N>::readTimeout(unsigned long maxDelay){
	if(!refReader.readTimeout(maxDelay)){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


template<int N>
void UnexpectedHandler<N>::handleSwitch(){
	
	if(refBuffer.remove(RING_STRING)){
		while(refBuffer.remove(RING_STRING)){}
		//change unexpectedMessages
		handleIncomingCall();
	}
	
	if(refBuffer.remove(NO_CARRIER_STRING)){
		
	}
	
	if(refBuffer.remove(UNDER_VOLTAGE_WARNING)){
		while(refBuffer.remove(UNDER_VOLTAGE_WARNING)){}
		
	}
	
	if(refBuffer.remove(OVER_VOLTAGE_WARNING)){
		while(refBuffer.remove(OVER_VOLTAGE_WARNING)){}
		
	}
	
	if(tcpHandler){
		if(refBuffer.remove(CONNECTE_OK)){
			tcpHandler->connectedSuccessfully();
		}
		
		// if(refBuffer.remove(ALREADY_CONNECTE)){
			// tcpHandler->connectedSuccessfully();
		// }
		
		if(refBuffer.remove(CONNECT_FAIL)){
			tcpHandler->connectionFaild();
		}
		
		if(refBuffer.remove(SHUT_OK)){
			tcpHandler->shutOk();
		}
		
		if(refBuffer.remove(CLOSED)){
			tcpHandler->closedConnection();
		}
		
		if(refBuffer.remove(INCOMING_MESSAGE)){
			tcpHandler->incomingMessage();
		}
	}

}


template<int N>
void UnexpectedHandler<N>::handleIncomingCall(){
	int lastIndex = refBuffer.indexOfEnd(NO_CARRIER_STRING);

	while(lastIndex == -1){
		readRemovingGarbage();
		lastIndex = refBuffer.indexOf(NO_CARRIER_STRING);
	}
	
	if(!refBuffer.remove(NO_CARRIER_STRING)){
		Serial.println("FALSE REMOVE");
	}
}


template<int N>
bool UnexpectedHandler<N>::readRemovingGarbage(){
	if(!refReader.read()){
		return false;
	}
	
	while(refBuffer.remove(RING_STRING)){}
	
	while(refBuffer.remove(UNDER_VOLTAGE_WARNING)){}
	
	while(refBuffer.remove(OVER_VOLTAGE_WARNING)){}
	
	return true;
}

