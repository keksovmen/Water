#include "HTTPHandler.h"
#include "Util.h"
#include "Enums.h"



HTTPHandler::HTTPHandler(
			SimCommandPort& simPort,
			SimResultParser& parser,
			SimState& state
			) :
	refPort(simPort), refParser(parser),
	refState(state)
{
	
	
}



bool HTTPHandler::initPostRequest(IPAddress& address, const char* url, int dataLength){
	if(initSession() 				&&
		setPostURL(address, url) 	&&
		setContentForPHP() 			&&
		startDataTransmition(dataLength))
	{
		return true;
	}
	
	return false;
}


bool HTTPHandler::initGetRequest(IPAddress& address, const char* url){
	if(initSession()){
		setGetURL(address, url);
		return true;
	}
	
	return false;
}


/**
	Tries to init http session
	if session is already open will terminate it
	and try to open new one
*/


bool HTTPHandler::initSession(){
	if(refState.http.isBussy){
		return false;
	}
	
	refPort.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	ANWSER_CODES code = readAndGetCode(refPort, refParser);
	switch (code){
		case OK:
			refState.http.isBussy = true;
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


bool HTTPHandler::setPostURL(IPAddress& address, const char* url){
	refPort.writeHTTPURL(address, url);
	return readAndExpectSuccess(refPort, refParser);
}


void HTTPHandler::setGetURL(IPAddress& address, const char* url){
	refPort.writeHTTPURL(address, url, false);
}


bool HTTPHandler::setContentForPHP(){
	refPort.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	return readAndExpectSuccess(refPort, refParser);
}


bool HTTPHandler::startDataTransmition(int dataLength){
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



bool HTTPHandler::terminateSession(){
	refPort.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	return readAndExpectSuccess(refPort, refParser);
}