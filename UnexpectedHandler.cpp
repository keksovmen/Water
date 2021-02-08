#include "UnexpectedHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"
#include <Arduino.h>



UnexpectedHandler::UnexpectedHandler(
				FixedBufferBase& buffer,
				BaseReader& reader,
				SimState& state
				) :
	refBuffer(buffer), refReader(reader),
	refState(state)
{

}




void UnexpectedHandler::attachTCPHandler(TCPHandler* tcpHandler){
	this->pTcpHandler = tcpHandler;
}




bool UnexpectedHandler::read(){
	if(!refReader.read()){
		return false;
	}
	
	handleSwitch();
	
	return true;
}



bool UnexpectedHandler::readTimeout(unsigned long maxDelay){
	if(!refReader.readTimeout(maxDelay)){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


bool UnexpectedHandler::lazyRead(){
	if(!refReader.lazyRead()){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


void UnexpectedHandler::handleSwitch(){
	
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
	
	if(refBuffer.remove(SAPBR_DEACT)){
		refState.diedGPRS();
	}
	
	if(refBuffer.remove(TEXT_RDY)){
		refState.readyDetected();
	}
	
	
	if(pTcpHandler){
		if(refBuffer.remove(TCP_CONNECTE_OK)){
			refState.tcp.state = TCP_STATE_CONNECTED;
		}
		
		if(refBuffer.remove(TCP_ALREADY_CONNECTE)){
			refState.tcp.state = TCP_STATE_CONNECTED;
		}
		
		if(refBuffer.remove(TCP_CONNECT_FAIL)){
			//there will be also \r\nSTATE: TCP CLOSED\r\n
			int index = refBuffer.indexOf("\r\nSTATE: ");
			int endIndex = refBuffer.indexOfFrom(index + 2, END_LINE);
			int amount = (endIndex - index) + strlen(END_LINE);
			refBuffer.remove(index, amount);
			
			refState.tcp.state = TCP_STATE_CLOSED;
		}
		
		//becuase i wait for it
		// if(refBuffer.remove(TCP_SHUT_OK)){
			// refState.tcp.state = TCP_STATE_INITIAL;
		// }
		
		if(refBuffer.remove(TCP_CONNECTION_CLOSED)){
			refState.tcp.state = TCP_STATE_CLOSED;
			refState.tcp.hastToSendId = true;
		}
		
		if(refBuffer.remove(PDP_DEACT)){
			refState.diedCGATT();
		}
		
		if(refBuffer.remove(TCP_INCOMING_MESSAGE)){
			refState.tcp.hasMessage = true;
		}
		
		
		if(refBuffer.remove(TCP_SEND_OK) ||
			refBuffer.remove(TCP_SEND_FAIL)
			)
		{
			refState.tcp.isSending = false;
		}
	}

}



void UnexpectedHandler::handleIncomingCall(){
	int lastIndex = refBuffer.indexOfEnd(NO_CARRIER_STRING);

	while(lastIndex == -1){
		readRemovingGarbage();
		lastIndex = refBuffer.indexOf(NO_CARRIER_STRING);
	}
	
	refBuffer.remove(NO_CARRIER_STRING);
}



bool UnexpectedHandler::readRemovingGarbage(){
	if(!refReader.read()){
		return false;
	}
	
	while(refBuffer.remove(RING_STRING)){}
	
	while(refBuffer.remove(UNDER_VOLTAGE_WARNING)){}
	
	while(refBuffer.remove(OVER_VOLTAGE_WARNING)){}
	
	return true;
}

