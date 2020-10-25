#include "HTTPHandler.h"
#include "Util.h"
#include "Enums.h"


template<int N>
HTTPHandler<N>::HTTPHandler(
			SimCommandPort& simPort,
			SimResultParser<N>& parser
			) :
	refPort(simPort), refParser(parser)
{
	
	
}


template<int N>
bool HTTPHandler<N>::initPostRequest(IPAddress& address, const char* url, int dataLength){
	if(initSession() 				&&
		setPostURL(address, url) 	&&
		setContentForPHP() 			&&
		startDataTransmition(dataLength))
	{
		return true;
	}
	
	return false;
}

template<int N>
bool HTTPHandler<N>::initGetRequest(){
	if(initSession()){
		setGetURL();
		return true;
	}
	
	return false;
}


/**
	Tries to init http session
	if session is already open will terminate it
	and try to open new one
*/

template<int N>
bool HTTPHandler<N>::initSession(){
	refPort.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	ANWSER_CODES code = readAndGetCode(refPort, refParser);
	switch (code){
		case OK:
			return true;
		
		case ERROR:
		case UNDEFINED:
			if(terminateSession()){
				refPort.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
				return readAndExpectSuccess(refPort, refParser);
			}else{
				return false;
			}
			
		default: break;
	}
	
	return false;
}

template<int N>
bool HTTPHandler<N>::setPostURL(IPAddress& address, const char* url){
	refPort.writeHTTPURL(address, url);
	return readAndExpectSuccess(refPort, refParser);
}

template<int N>
void HTTPHandler<N>::setGetURL(){
	refPort.writeHTPPSetParam("URL", nullptr);
}

template<int N>
bool HTTPHandler<N>::setContentForPHP(){
	refPort.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	return readAndExpectSuccess(refPort, refParser);
}

template<int N>
bool HTTPHandler<N>::startDataTransmition(int dataLength){
	refPort.writeHTPPData(dataLength);
	if(!refPort.read()){
		return false;
	}
	
	if(!refParser.containDownload()){
		if(!refPort.read()){
			return false;
		}
		if(!refParser.containDownload()){
			return false;
		}
	}
	return true;
}


template<int N>
bool HTTPHandler<N>::terminateSession(){
	refPort.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	return readAndExpectSuccess(refPort, refParser);
}