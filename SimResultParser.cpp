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
	//TODO: if contain other messages you have to check
	//something like: from end to start looking for 0 or 4 \r\n
	return buffer.endsWith("\r\n");
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
	//TODO: osntead of endWith use indexOf()
	return buffer.endsWith("\r\n0\r\n") ||	//success code
			buffer.endsWith("\r\n4\r\n");	//error code
}


template<int N>
bool SimResultParser<N>::containDownload(){
	if(buffer.indexOf("DOWNLOAD") == -1){
		return false;
	}
	return true;
}


template<int N>
int SimResultParser<N>::fetchResultCode(){
	//TODO: use find indexOf \r\n(0 or 4)\r\n
	//to proper handling
	buffer.trim();
	const char* last = buffer.end() - 1;
	
	if(isDigit(*last)){
		int result = characterToInt(*last);
		buffer--;
		return result;
	}
	
	return ANWSER_CODES::UNDEFINED;
}


template<int N>
int SimResultParser<N>::fetchSimpleTextCode(){
	//TODO: use find indexOf \r\n(OK)\r\n
	//to proper handling
	buffer.trim();
	if(buffer.endsWith("OK")){
		buffer--;
		buffer--;
		return ANWSER_CODES::OK;
	}
	
	if(buffer.endsWith("ERROR")){
		return ANWSER_CODES::ERROR;
	}
	
	return ANWSER_CODES::UNDEFINED;
}


/**
	Expects buffer as +CREG:<n>,<stat>\r\n
	And expect <n> = 0
*/

template<int N>
int SimResultParser<N>::fetchNetworkRegistration(){
	buffer.trim();
	//TODO: use instead of atoi characterToInt() from Util.h
	//use indexOf to proper handling
	int code = atoi(buffer.end() - 1);
	buffer--;
	
	return code;
}


/**
	Message looks like:
	
	+SAPBR: cid,status
	
		cid - index of barier
		status:
			0 - connecting
			1 - connected
			2 - closing
			3 - closed
*/

template<int N>
int SimResultParser<N>::fetchGPRSStatus(){
	//TODO: use indexOf
	buffer.trim();
	buffer.remove(0, 10);
	return characterToInt(buffer[0]);
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
	buffer.trim();
	int index = buffer.indexOf("+HTTPACTION: ");
	if(index == -1){
		return -1;
		//error
	}
	
	index += 12;	//length of HTTPACTION:\s, index on \s
	index += 3;		//<method> and ',', so now on <status> should be
	return characterToInt(buffer[index]);
}
