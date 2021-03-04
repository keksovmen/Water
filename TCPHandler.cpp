#include "TCPHandler.h"

#include <Arduino.h>
#include "Util.h"
#include "StringData.h"



TCPHandler::TCPHandler(
				SimTools& tools,
				ParameterHandler& parameters
				) :
	refTools(tools), refParameters(parameters)
{
	
}



bool TCPHandler::handle(){
	if(isLastCommandCIICR){
		if(refTools.isSimpleMessageReady()){
			if(refTools.fetchResultCode() == OK){
				refTools.state.tcp.state = TCP_STATE_IP_GPRS_ACT;
			}else{
				refTools.state.tcp.state = TCP_STATE_UNDEFINIED;
			}
			
			return true;
		}
		
		return false;
	}else{
		if(refTools.containShut()){
			refTools.state.tcp.state = TCP_STATE_INITIAL;
			return true;
		}else{
			//check if there is ERROR if no return false
			ANWSER_CODES code = static_cast<ANWSER_CODES>(
							refTools.fetchResultCode()
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


void TCPHandler::handleUnexpected(FixedBufferBase& refBuffer){
	if(refBuffer.remove(TCP_CONNECTE_OK)){
		refTools.state.tcp.state = TCP_STATE_CONNECTED;
	}
		
	if(refBuffer.remove(TCP_ALREADY_CONNECTE)){
		refTools.state.tcp.state = TCP_STATE_CONNECTED;
	}
	
	//TODO: bug here because you are not having 100%
	//chance to see next message full
	if(refBuffer.remove(TCP_CONNECT_FAIL)){
		//there will be also \r\nSTATE: TCP CLOSED\r\n
		int index = refBuffer.indexOf("\r\nSTATE: ");
		int endIndex = refBuffer.indexOfFrom(index + 2, END_LINE);
		int amount = (endIndex - index) + strlen(END_LINE);
		refBuffer.remove(index, amount);
		
		refTools.state.tcp.state = TCP_STATE_CLOSED;
	}
		
	if(refBuffer.remove(TCP_CONNECTION_CLOSED)){
		refTools.state.tcp.state = TCP_STATE_CLOSED;
		refTools.state.tcp.hastToSendId = true;
	}
		
	if(refBuffer.remove(PDP_DEACT)){
		refTools.state.diedCGATT();
	}
		
	if(refBuffer.remove(TCP_INCOMING_MESSAGE)){
		refTools.state.tcp.hasMessage = true;
	}
		
		
	if(refBuffer.remove(TCP_SEND_OK) ||
		refBuffer.remove(TCP_SEND_FAIL)
		)
	{
		refTools.state.tcp.isSending = false;
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
			//Send my id
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
	refTools.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_INFO);
	int length = 0;
	
	if(refTools.readAndExpectSuccess()){
		length = refTools.parseRxGetLength();
	}else{
		Serial.println("ERROR NO ANWSER FROM LENGTH");
	}
	
	return TCPReader(refTools, buffer, length);
}


void TCPHandler::sendPong(){
	if(!initSending()){
		return;
	}
	
	refTools.write("0\n");
	refTools.write((char) 0x1A);
	//TODO: don't know what to do, maybe change state to 
	//something intermidiate like waiting for result
	refTools.state.tcp.hasToSendPong = false;
	
}


void TCPHandler::sendId(){
	if(!initSending()){
		return;
	}
	
	refTools.write(refParameters.getPlateId().getValue().getValue());
	refTools.write("\n");
	refTools.write(refParameters.getImei().getValue().getValue());
	refTools.write("\n");
	refTools.write((char) 0x1A);
	
	refTools.state.tcp.hastToSendId = false;
}


void TCPHandler::sendAcknowledgment(){
	if(!initSending()){
		return;
	}
	
	refTools.write("1\n");
	refTools.write((char) 0x1A);
	
	refTools.state.tcp.hasToSendAcknowledgment = false;
}



bool TCPHandler::handleInitial(){
	refTools.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_MODE);
	bool currentStatus = false;
	
	if(refTools.readAndExpectSuccess()){
		currentStatus = refTools.fetchRxGetStatus();
	}else{
		return false;
	}
	
	if(!currentStatus){
		refTools.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_ON);
		if(!refTools.readAndExpectSuccess()){
			return false;
		}
	}
	
	refTools.writeCSTT(
		refParameters.getApn().getValue().getValue()
		);
	
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handleIpStart(){
	refTools.writeCIICR();
	refTools.state.longCmd.cmdHandler = this;
	isLastCommandCIICR = true;
	
	return true;
}



bool TCPHandler::handleGPRSAct(){
	refTools.writeGetIpTCP();
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handleIpStatus(){
	refTools.writeCIPSTART(
			refParameters.getAddress().getValue(),
			//TODO: made as parameter
			8188
			);
	
	return refTools.readAndExpectSuccess();
}



bool TCPHandler::handlePDPDeact(){
	refTools.writeCIPSHUT();
	// refTools.state.longCmd.cmdHandler = this;
	refTools.state.setLongCmd(this);
	isLastCommandCIICR = false;
	
	return true;
}



TCP_STATE TCPHandler::handleUndefinied(){
	refTools.writeCIPSTATUS();
	if(refTools.readAndExpectSuccess()){
		return refTools.fetchTCPState();
	}
	
	return TCP_STATE_UNDEFINIED;
}


bool TCPHandler::initSending(){
	refTools.writeCIPSEND();
	if(!refTools.readTimeout(LONG_WAIT)){
		return false;
	}
	
	refTools.state.tcp.isSending = true;
	return true;
}