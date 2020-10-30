#include "UnexpectedHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"
#include <Arduino.h>


template<int N>
UnexpectedHandler<N>::UnexpectedHandler(
				FixedBuffer<N>& buffer,
				BaseReader& reader,
				SimState& state
				) :
	refBuffer(buffer), refReader(reader),
	refState(state)
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
	
	if(refBuffer.remove(SAPBR_DEACT)){
		refState.diedGPRS();
	}
	
	
	if(tcpHandler){
		if(refBuffer.remove(TCP_CONNECTE_OK)){
			refState.tcp.state = TCP_STATE_CONNECTED;
		}
		
		if(refBuffer.remove(TCP_ALREADY_CONNECTE)){
			refState.tcp.state = TCP_STATE_CONNECTED;
		}
		
		if(refBuffer.remove(TCP_CONNECT_FAIL)){
			refState.tcp.state = TCP_STATE_CLOSED;
		}
		
		//becuase i wait for it
		// if(refBuffer.remove(TCP_SHUT_OK)){
			// refState.tcp.state = TCP_STATE_INITIAL;
		// }
		
		if(refBuffer.remove(TCP_CONNECTION_CLOSED)){
			refState.tcp.state = TCP_STATE_CLOSED;
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


template<int N>
void UnexpectedHandler<N>::handleIncomingCall(){
	int lastIndex = refBuffer.indexOfEnd(NO_CARRIER_STRING);

	while(lastIndex == -1){
		readRemovingGarbage();
		lastIndex = refBuffer.indexOf(NO_CARRIER_STRING);
	}
	
	refBuffer.remove(NO_CARRIER_STRING);
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

