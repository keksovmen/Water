#include "SimResultParser.h"

#include <New.h>
#include <Arduino.h>
#include "Util.h"
#include "StringData.h"



static char dynamicMemory[6];




SimResultParser::SimResultParser(FixedBufferBase& refBuffer) :
	ResultParserStateBase(refBuffer)
{
	pState = new(dynamicMemory) ResultParserStateBase(refBuffer);
	
}



void SimResultParser::setState(PARSER_STATE state){
	switch(state){
		case PARSER_STATE_TEXT:
			pState = new (dynamicMemory) ResultParserStateText(this->refBuffer);
			break;
			
		case PARSER_STATE_DIGIT:
			pState = new (dynamicMemory) ResultParserStateDigit(this->refBuffer);
			break;
			
		case PARSER_STATE_BOTH:
			pState = new (dynamicMemory) ResultParserStateBase(this->refBuffer);
			break;
	}
}


/**
	Minimum responce length is 3 characters
	
	If ATV0 then anwser coud look like:
		Information response: 		<text><CR><LF>
		Short result code format: 	<numeric code><CR>
			But it also adds <LF> at the end, don't know why?
	If ATV1 ->:
		Information response: 		<CR><LF><text><CR><LF>
		Long result code format: 	<CR><LF><verbose code><CR><LF>
		
	<CR> in ASCI = 13 = '\r'
	<LF> in ASCI = 10 = '\n'
*/


bool SimResultParser::isSimpleMessageReady(){
	return pState->isSimpleMessageReady();
}


/**
	Some commands return data and result code
	So length must be more than 3 symbols
	
	e.x.: AT+CSQ, AT+CREG?
	//TODO: made some check when ATV1
*/


bool SimResultParser::isComplexMessageReady(){
	return pState->isComplexMessageReady();
}



bool SimResultParser::isReadMessageFull(READ_TYPE type){
	return pState->isReadMessageFull(type);
}



int SimResultParser::fetchResultCode(){
	return pState->fetchResultCode();
}



void SimResultParser::removeReadGarbage(READ_TYPE type){
	pState->removeReadGarbage(type);
}



bool SimResultParser::containDownload(){
	if(this->refBuffer.indexOf("DOWNLOAD\r\n") != -1)
		return true;
	
	return false;
}



bool SimResultParser::isHttpActionPresents(){
	int index = this->refBuffer.indexOf(HTTP_HTTPACTION);
	if(index == -1)
		return false;
	
	int terminatorIndex = this->refBuffer.indexOfFrom(
			index + strlen(HTTP_HTTPACTION), END_LINE);
	if(terminatorIndex == -1)
		return false;
	
	return true;
}


bool SimResultParser::checkError(){
	return pState->checkError();
}


/**
	Expects this->refBuffer as +CREG: <n>,<stat>\r\n
		<n> = 0...2
		<stat> = 0...5
*/


int SimResultParser::fetchNetworkRegistration(){
	int index = this->refBuffer.indexOf("+CREG: ");
	index += 9;	//move on <stat> position
	
	return characterToInt(this->refBuffer[index]);
}


/**
	Message looks like:
	
	+SAPBR: cid,status
	
		cid - index of barier - 0...4
		status:
			0 - connecting
			1 - connected
			2 - closing
			3 - closed
*/


int SimResultParser::fetchGPRSStatus(){
	int index = this->refBuffer.indexOf("+SAPBR: ");
	index += 10;
	
	return characterToInt(this->refBuffer[index]);
}


/**
	Message looks loke:
	
	+HTTPACTION: <method>,<status>,<data length>
		method - 0 - GET, 1 - POST
		status - 3 digit as real http status code
		data length - amount of recieved data from server

*/


int SimResultParser::fetchHTTPStatus(){
	int index = this->refBuffer.indexOf(HTTP_HTTPACTION);
	index += strlen(HTTP_HTTPACTION);	//index on method
	index += 2;	//index will be on first character of status
	
	
	return characterToInt(this->refBuffer[index]);
}


/**
	Message looks loke:
	
	+HTTPACTION: <method>,<status>,<data length>\r\n
		method - 0 - GET, 1 - POST
		status - 3 digit as real http status code
		data length - amount of recieved data from server
	

*/


unsigned long SimResultParser::fetchHttpResponceLength(){
	int index = this->refBuffer.indexOf(HTTP_HTTPACTION);
	index += strlen(HTTP_HTTPACTION);	//index on method
	index += 6;	//on first char of <data length>
	
	int endIndex = this->refBuffer.indexOfFrom(index, "\r\n");
	
	int j = 0;
	char storage[7];	//max length is 6 char and 7th for \0
	for(int i = index; i < endIndex; i++, j++){
		storage[j] = this->refBuffer[i];
	}
	
	storage[j] = '\0';
	char* end;
	return strtol(storage, &end, 10);
}


bool SimResultParser::isPinRdy(){
	int index = this->refBuffer.indexOf("+CPIN: READY\r\n");
	return index != -1;
}



bool SimResultParser::isPossibleMessage(){
	return (this->refBuffer.indexOfEnd(END_LINE) != -1) ||
			this->refBuffer.getLength() > 0;
}



bool SimResultParser::isAttachedToGPRSServices(){
	int index = this->refBuffer.indexOf("+CGATT: ");
	return characterToInt(this->refBuffer[index + 8]) == 1 
			? true : false;
}



TCP_STATE SimResultParser::fetchTCPState(){
	int index = this->refBuffer.indexOf("STATE: ");
	index += 7;		//move index on first letter of actual state
	
	if(this->refBuffer.indexOfFrom(index, "INITIAL") != -1){
		return TCP_STATE::TCP_STATE_INITIAL;
	}
	
	if(this->refBuffer.indexOfFrom(index, "START") != -1){
		return TCP_STATE::TCP_STATE_IP_START;
	}
	
	if(this->refBuffer.indexOfFrom(index, "CONFIG") != -1){
		return TCP_STATE::TCP_STATE_IP_CONFIG;
	}
	
	if(this->refBuffer.indexOfFrom(index, "GPRSACT") != -1){
		return TCP_STATE::TCP_STATE_IP_GPRS_ACT;
	}
	
	if(this->refBuffer.indexOfFrom(index, "STATUS") != -1){
		return TCP_STATE::TCP_STATE_IP_STATUS;
	}
	
	if(this->refBuffer.indexOfFrom(index, "CONNECTING") != -1){
		return TCP_STATE::TCP_STATE_CONNECTING;
	}
	
	if(this->refBuffer.indexOfFrom(index, "CONNECT OK") != -1){
		return TCP_STATE::TCP_STATE_CONNECTED;
	}
	
	if(this->refBuffer.indexOfFrom(index, "CLOSING") != -1){
		return TCP_STATE::TCP_STATE_CLOSING;
	}
	
	if(this->refBuffer.indexOfFrom(index, "CLOSED") != -1){
		return TCP_STATE::TCP_STATE_CLOSED;
	}
	
	if(this->refBuffer.indexOfFrom(index, "PDP DEACT") != -1){
		return TCP_STATE::TCP_STATE_PDP_DEACT;
	}
	
	return TCP_STATE::TCP_STATE_UNDEFINIED;
}



int SimResultParser::fetchRxGetStatus(){
	int index = this->refBuffer.indexOf("+CIPRXGET: ");
	return characterToInt(this->refBuffer[index + 11]);
}



int SimResultParser::parseRxGetLength(){
	int index = this->refBuffer.indexOf(TCP_DATA_LENGTH_ANWSER);
	index += strlen(TCP_DATA_LENGTH_ANWSER);
	
	return atoi(&this->refBuffer[index]);
}



bool SimResultParser::containShut(){
	int index = this->refBuffer.indexOf(TCP_SHUT_OK);
	return index != -1;
}
