#include "SimResultParser.h"
#include "Enums.h"
#include "Util.h"

const static char* DIGIT_SUCCESS = "0\r\n";
const static char* DIGIT_COMPLEX_SUCCESS = "\r\n0\r\n";

const static char* DIGIT_ERROR = "4\r\n";
const static char* DIGIT_COMPLEX_ERROR = "\r\n4\r\n";

const static char* TEXT_SUCCESS = "\r\nOK\r\n";
const static char* TEXT_ERROR = "\r\nERROR\r\n";

const static char* END_LINE = "\r\n";


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
	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;
	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
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
	
	if(buffer.indexOfEnd(DIGIT_COMPLEX_SUCCESS) != -1)
		return true;
	if(buffer.indexOfEnd(DIGIT_COMPLEX_ERROR) != -1)
		return true;
	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;
	
	return false;
}


template<int N>
bool SimResultParser<N>::containDownload(){
	if(buffer.indexOf("DOWNLOAD\r\n") != -1)
		return true;
	
	return false;
}


template<int N>
bool SimResultParser<N>::isHttpActionPresents(){
	int index = buffer.indexOf("+HTTPACTION: ");
	if(index == -1)
		return false;
	
	int terminatorIndex = buffer.indexOfFrom(index + 11, END_LINE);
	if(terminatorIndex == -1)
		return false;
	
	return true;
}


/**
	MEssage looks like 
	\r\n+HTTPREAD: <n>\r\n
		<n> = amount of read data
*/

template<int N>
bool SimResultParser<N>::isReadHttpMessageFull(){
	int index = buffer.indexOf("\r\n+HTTPREAD: ");
	int endIndex = buffer.indexOfFrom(index + 11, END_LINE);
	
	if(endIndex == -1){
		return false;
	}
	
	if(endIndex == buffer.indexOfEnd(END_LINE)){
		return false;
	}
	
	return isSimpleMessageReady();
}


template<int N>
int SimResultParser<N>::fetchResultCode(){
	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;
	
	return ANWSER_CODES::UNDEFINED;
}


/**
	Expects buffer as +CREG: <n>,<stat>\r\n
		<n> = 0...2
		<stat> = 0...5
*/

template<int N>
int SimResultParser<N>::fetchNetworkRegistration(){
	int index = buffer.indexOf("+CREG: ");
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
	index += 15;	//index will be on first character of status
	
	return characterToInt(buffer[index]);
}


/**
	Message looks loke:
	
	+HTTPACTION: <method>,<status>,<data length>\r\n
		method - 0 - GET, 1 - POST
		status - 3 digit as real http status code
		data length - amount of recieved data from server
	

*/

template<int N>
unsigned long SimResultParser<N>::fetchHttpResponceLength(){
	int index = buffer.indexOf("+HTTPACTION: ");	
	index += 19;	//on first char of <data length>
	
	int endIndex = buffer.indexOfFrom(index, "\r\n");
	
	int j = 0;
	char storage[7];	//max length is 6 char and 7th for \0
	for(int i = index; i < endIndex; i++, j++){
		storage[j] = buffer[i];
	}
	
	storage[j] = '\0';
	char* end;
	return strtol(storage, &end, 10);
}


template<int N>
void SimResultParser<N>::removeReadHttpGarbage(){
	int index = buffer.indexOf("\r\n+HTTPREAD: ");
	int endIndex = buffer.indexOfFrom(index + 10, END_LINE);
	endIndex += 2;
	buffer.remove(index, endIndex - index);
	
	index = buffer.indexOfEnd(DIGIT_SUCCESS);
	buffer.remove(index, 3);
}


template<int N>
bool SimResultParser<N>::isPinRdy(){
	int index = buffer.indexOf("+CPIN: READY\r\n");
	return index != -1;
}
