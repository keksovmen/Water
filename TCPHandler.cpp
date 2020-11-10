#include "TCPHandler.h"

#include <Arduino.h>
#include "Util.h"




TCPHandler::TCPHandler(
				SimTools& tools,
				ParameterHandler& parameters
				) :
	refTools(tools), refParameters(parameters)
{
	
}



bool TCPHandler::handle(){
	if(isLastCommandCIICR){
		if(refTools.parser.isSimpleMessageReady()){
			if(refTools.parser.fetchResultCode() == OK){
				refTools.state.tcp.state = TCP_STATE_IP_GPRS_ACT;
			}else{
				refTools.state.tcp.state = TCP_STATE_UNDEFINIED;
			}
			
			return true;
		}
		
		return false;
	}else{
		if(refTools.parser.containShut()){
			refTools.state.tcp.state = TCP_STATE_INITIAL;
			return true;
		}else{
			//check if there is ERROR if no return false
			ANWSER_CODES code = static_cast<ANWSER_CODES>(
							refTools.parser.fetchResultCode()
							);
			switch(code){
				case ERROR:
					refTools.state.tcp.state = TCP_STATE_UNDEFINIED;
					return true;
					
				default: return false;
			}
		}
	}
}



bool TCPHandler::connect(){
	switch(refTools.state.tcp.state){
		case TCP_STATE_INITIAL :
			if(handleInitial()){
				refTools.state.tcp.state = TCP_STATE_IP_START;
			}
			
			break;
			
		case TCP_STATE_IP_START: 
			if(handleIpStart()){
				refTools.state.tcp.state = TCP_STATE_IP_CONFIG;
				return true;
			}
			
			break;
			
		case TCP_STATE_IP_CONFIG:
			//wait until handle will be called
			// tryUpdateState();
			break;
			
		case TCP_STATE_IP_GPRS_ACT: 
			if(handleGPRSAct()){
				refTools.state.tcp.state = TCP_STATE_IP_STATUS;
			}else{
				refTools.state.tcp.state = TCP_STATE_UNDEFINIED;
			}
			break;
			
		case TCP_STATE_IP_STATUS:
			if(handleIpStatus()){
				//fix when UnexpectedHandler sets state of greater power
				if(refTools.state.tcp.state == TCP_STATE_CONNECTED){
					break;
				}
				
				refTools.state.tcp.state = TCP_STATE_CONNECTING;
			}else{
				refTools.state.tcp.state = TCP_STATE_UNDEFINIED;
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
			refTools.state.tcp.state = TCP_STATE_IP_STATUS;
			break;
			
		case TCP_STATE_PDP_DEACT:
			if(handlePDPDeact()){
				return true;
			}
			break;
			
		case TCP_STATE_UNDEFINIED:
			refTools.state.tcp.state = handleUndefinied();
			break;
	}
	
	return false;
}



TCPReader TCPHandler::readMessage(FixedBufferBase& buffer){
	refTools.simPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_INFO);
	int length = 0;
	
	if(refTools.readAndExpectSuccess()){
		length = refTools.parser.parseRxGetLength();
	}else{
		Serial.println("ERROR NO ANWSER FROM LENGTH");
	}
	
	return TCPReader(refTools, buffer, length);
}



bool TCPHandler::handleInitial(){
	refTools.simPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_MODE);
	bool currentStatus = false;
	
	if(refTools.readAndExpectSuccess()){
		currentStatus = refTools.parser.fetchRxGetStatus();
	}else{
		return false;
	}
	
	if(!currentStatus){
		refTools.simPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_ON);
		if(!refTools.readAndExpectSuccess()){
			return false;
		}
	}
	
	refTools.simPort.writeCSTT(
		refParameters.getApn().getValue().getValue()
		);
	
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handleIpStart(){
	refTools.simPort.writeCIICR();
	refTools.state.longCmd.cmdHandler = this;
	isLastCommandCIICR = true;
	
	return true;
}



bool TCPHandler::handleGPRSAct(){
	refTools.simPort.writeGetIpTCP();
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handleIpStatus(){
	refTools.simPort.writeCIPSTART(
			refParameters.getAddress().getValue(),
			8188
			);
	
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handlePDPDeact(){
	refTools.simPort.writeCIPSHUT();
	// refTools.state.longCmd.cmdHandler = this;
	refTools.state.setLongCmd(this);
	isLastCommandCIICR = false;
	
	return true;
}



TCP_STATE TCPHandler::handleUndefinied(){
	refTools.simPort.writeCIPSTATUS();
	if(refTools.readAndExpectSuccess()){
		return refTools.parser.fetchTCPState();
	}
	
	return TCP_STATE_UNDEFINIED;
}