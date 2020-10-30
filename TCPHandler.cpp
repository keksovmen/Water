#include "TCPHandler.h"

#include <Arduino.h>
#include "Util.h"



template<int N>
TCPHandler<N>::TCPHandler(
				SimCommandPort& simPort,
				SimResultParser<N>& parser,
				ParameterHandler& parameters,
				SimState& state
				) :
	refPort(simPort), refParser(parser),
	refParameters(parameters), refState(state)
{
	
}


template<int N>
bool TCPHandler<N>::handle(){
	if(isLastCommandCIICR){
		if(refParser.isSimpleMessageReady()){
			if(refParser.fetchResultCode() == OK){
				refState.tcp.state = TCP_STATE_IP_GPRS_ACT;
			}else{
				refState.tcp.state = TCP_STATE_UNDEFINIED;
			}
			
			return true;
		}
		
		return false;
	}else{
		if(refParser.containShut()){
			refState.tcp.state = TCP_STATE_INITIAL;
			return true;
		}else{
			//check if there is ERROR if no return false
			ANWSER_CODES code = static_cast<ANWSER_CODES>(
							refParser.fetchResultCode()
							);
			switch(code){
				case ERROR:
					refState.tcp.state = TCP_STATE_UNDEFINIED;
					return true;
					
				default: return false;
			}
		}
	}
}


template<int N>
bool TCPHandler<N>::connect(){
	switch(refState.tcp.state){
		case TCP_STATE_INITIAL :
			if(handleInitial()){
				refState.tcp.state = TCP_STATE_IP_START;
			}
			
			break;
			
		case TCP_STATE_IP_START: 
			if(handleIpStart()){
				refState.tcp.state = TCP_STATE_IP_CONFIG;
				return true;
			}
			
			break;
			
		case TCP_STATE_IP_CONFIG:
			//wait until handle will be called
			// tryUpdateState();
			break;
			
		case TCP_STATE_IP_GPRS_ACT: 
			if(handleGPRSAct()){
				refState.tcp.state = TCP_STATE_IP_STATUS;
			}else{
				refState.tcp.state = TCP_STATE_UNDEFINIED;
			}
			break;
			
		case TCP_STATE_IP_STATUS:
			if(handleIpStatus()){
				//fix when UnexpectedHandler sets state of greater power
				if(refState.tcp.state == TCP_STATE_CONNECTED){
					break;
				}
				
				refState.tcp.state = TCP_STATE_CONNECTING;
			}else{
				refState.tcp.state = TCP_STATE_UNDEFINIED;
			}
			break;
			
		case TCP_STATE_CONNECTING:
			//just wait until status will be changed
			break;
			
		case TCP_STATE_CONNECTED:
			//congrats you are connected
			break;
			
		case TCP_STATE_CLOSING:
			//just wait until status will be changed
			break;
		
		case TCP_STATE_CLOSED:
			refState.tcp.state = TCP_STATE_IP_STATUS;
			break;
			
		case TCP_STATE_PDP_DEACT:
			if(handlePDPDeact()){
				return true;
			}
			break;
			
		case TCP_STATE_UNDEFINIED:
			refState.tcp.state = handleUndefinied();
			break;
	}
	
	return false;
}


template<int N>
TCPReader<N> TCPHandler<N>::readMessage(FixedBuffer<N> buffer){
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_INFO);
	int length = 0;
	
	if(readAndExpectSuccess(refPort, refParser)){
		length = refParser.parseRxGetLength();
	}else{
		Serial.println("ERROR NO ANWSER FROM LENGTH");
	}
	
	return TCPReader<N>(refParser, refPort, buffer, length);
}


template<int N>
bool TCPHandler<N>::handleInitial(){
	refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_MODE);
	bool currentStatus = false;
	
	if(readAndExpectSuccess(refPort, refParser)){
		currentStatus = refParser.fetchRxGetStatus();
	}else{
		return false;
	}
	
	if(!currentStatus){
		refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_ON);
		if(!readAndExpectSuccess(refPort, refParser)){
			return false;
		}
	}
	
	refPort.writeCSTT("internet");	//take from params
	
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
bool TCPHandler<N>::handleIpStart(){
	refPort.writeCIICR();
	refState.longCmd.cmdHandler = this;
	isLastCommandCIICR = true;
	
	return true;
}


template<int N>
bool TCPHandler<N>::handleGPRSAct(){
	refPort.writeGetIpTCP();
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
bool TCPHandler<N>::handleIpStatus(){
	refPort.writeCIPSTART(
			refParameters.getAddress().getValue(),
			8188
			);
	
	return readAndExpectSuccess(refPort, refParser);
}


template<int N>
bool TCPHandler<N>::handlePDPDeact(){
	refPort.writeCIPSHUT();
	refState.longCmd.cmdHandler = this;
	isLastCommandCIICR = false;
	
	return true;
}


template<int N>
TCP_STATE TCPHandler<N>::handleUndefinied(){
	refPort.writeCIPSTATUS();
	if(readAndExpectSuccess(refPort, refParser)){
		return refParser.fetchTCPState();
	}
	
	return TCP_STATE_UNDEFINIED;
}