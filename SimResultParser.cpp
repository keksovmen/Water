#include "SimResultParser.h"
#include "Enums.h"
#include "Util.h"


template<int N>
SimResultParser<N>::SimResultParser(FixedBuffer<N>& refBuffer) :
	buffer(refBuffer){
	
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

template<int N>
bool SimResultParser<N>::isSimpleMessageReady(){
	if(buffer.indexOfEnd("0\r\n") != -1)
		return true;
	if(buffer.indexOfEnd("4\r\n") != -1)
		return true;
	if(buffer.indexOfEnd("\r\nOK\r\n") != -1)
		return true;
	if(buffer.indexOfEnd("\r\nERROR\r\n") != -1)
		return true;
	
	return false;
}


/**
	Some commands return data and result code
	So length must be more than 3 symbols
	
	e.x.: AT+CSQ, AT+CREG?
	//TODO: made some check when ATV1
*/

template<int N>
bool SimResultParser<N>::isComplexMessageReady(){
	if(buffer.getLength() <= 3){
		return false;
	}
	
	if(buffer.indexOfEnd("\r\n0\r\n") != -1)
		return true;
	if(buffer.indexOfEnd("\r\n4\r\n") != -1)
		return true;
	
	return false;
}


template<int N>
bool SimResultParser<N>::containDownload(){
	if(buffer.indexOf("DOWNLOAD") != -1)
		return true;
	
	return false;
}


template<int N>
bool SimResultParser<N>::isHttpActionPresents(){
	int index = buffer.indexOf("+HTTPACTION: ");
	if(index == -1)
		return false;
	
	int terminatorIndex = buffer.indexOfEnd("\r\n");
	if(terminatorIndex == -1)
		return false;
	
	if(terminatorIndex < index)
		return false;
	
	return true;
}


template<int N>
int SimResultParser<N>::fetchResultCode(){
	int index = buffer.indexOfEnd("0\r\n");
	
	if(index == -1){
		index = buffer.indexOfEnd("4\r\n");
		
		if(index == -1){
			return ANWSER_CODES::UNDEFINED;
		}
	}
	
	int result = characterToInt(buffer[index]);
	buffer.remove(index, 3);
	
	return result;
}


template<int N>
int SimResultParser<N>::fetchSimpleTextCode(){
	//TODO: look realy hard to understand remove if else somehow
	int index = buffer.indexOfEnd("\r\nOK\r\n");
	
	if(index == -1){
		index = buffer.indexOfEnd("\r\nERROR\r\n");
		
		if(index == -1){
			return ANWSER_CODES::UNDEFINED;
			
		}else{
			buffer.remove(index, 9);
			
			return ANWSER_CODES::ERROR;
		}
		
	}else{
		buffer.remove(index, 6);
		
		return ANWSER_CODES::ERROR;
	}
}


/**
	Expects buffer as +CREG: <n>,<stat>\r\n
		<n> = 0...2
		<stat> = 0...5
*/

template<int N>
int SimResultParser<N>::fetchNetworkRegistration(){
	int index = buffer.indexOf("+CREG: ");
	
	if(index == -1){
		//display error some how
		return -1;
	}
	
	index += 9;	//move on <stat> position
	
	return characterToInt(buffer[index]);
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

template<int N>
int SimResultParser<N>::fetchGPRSStatus(){
	int index = buffer.indexOf("+SAPBR: ");
	
	if(index == -1){
		//indicate error somehow
		return -1;
	}
	
	index += 10;
	
	return characterToInt(buffer[index]);
}


/**
	Message looks loke:
	
	+HTTPACTION: <method>,<status>,<data length>
		method - 0 - GET, 1 - POST
		status - 3 digit as real http status code
		data length - amount of recieved data from server

*/

template<int N>
int SimResultParser<N>::fetchHTTPStatus(){
	int index = buffer.indexOf("+HTTPACTION: ");
	
	if(index == -1){
		//indicate error
		return -1;
	}
	
	index += 15;	//index will be on first character of status
	
	return characterToInt(buffer[index]);
}
