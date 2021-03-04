#include "TCPIncomingHandler.h"
#include <Arduino.h>
#include "Constants.h"
#include "ParameterHandler.h"


TCPIncomingHandler::TCPIncomingHandler(
							FixedBufferBase& refBuffer,
							SimTools& tools
							) :
	refBuffer(refBuffer), refTools(tools){}


void TCPIncomingHandler::handleMessage()
{
	while(hasCommand()){
		int begining = getBeginingOfCommand();
		TCP_MESSAGE_TYPE type = getCommandType(begining);
		handleCommand(type, begining);
		removeCommand(begining);
	}
}


bool TCPIncomingHandler::hasCommand(){
	int begining = getBeginingOfCommand();
	int end = refBuffer.indexOf(ENTRY_ENDING);
	
	return begining != -1 && 
				end != -1;
}


int TCPIncomingHandler::getBeginingOfCommand(){
	return refBuffer.indexOf(ENTRY_BEGINING);
}


TCP_MESSAGE_TYPE TCPIncomingHandler::getCommandType(int begining){
	if(refBuffer.indexOfFrom(begining, "PING") != -1){
		return TCP_MESSAGE_TYPE_PING;
	}
	
	return TCP_MESSAGE_TYPE_PARAM;
}

void TCPIncomingHandler::handleCommand(TCP_MESSAGE_TYPE cmd, int begining){
	switch(cmd){
		case TCP_MESSAGE_TYPE_PARAM:
			handleSetParam(begining);
			refTools.state.tcp.hasToSendAcknowledgment = true;
			break;
			
		case TCP_MESSAGE_TYPE_PING:
			handlePing();
			break;
	}
}


void TCPIncomingHandler::removeCommand(int begining){
	int end = refBuffer.indexOfFrom(begining, ENTRY_ENDING);
	int amount = (end - begining) + 1;
	refBuffer.remove(begining, amount);
}


void TCPIncomingHandler::handleSetParam(int begining){
	int id = atoi(&refBuffer[begining + 1]);
	int dataIndex = refBuffer.indexOfFrom(begining, "=");
	dataIndex++;
	ParameterHandler& refParams = ParameterHandler::getInstance();
	if(refParams.hasId(id)){
		refParams.parse(id, &refBuffer[dataIndex]);
	}
}


void TCPIncomingHandler::handlePing(){
	refTools.state.tcp.hasToSendPong = true;
}