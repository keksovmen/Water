#include "HTTPHandler.h"
#include "Util.h"
#include "Constants.h"



HTTPHandler::HTTPHandler(SimTools& tools) :
	refTools(tools)
{
	
	
}



bool HTTPHandler::initPostRequest(IPAddress& address, const char* url, int dataLength){
	if(initSession() 				&&
		setPostURL(address, url) 	&&
		#ifdef SIM800
		#warning SIM800 defined
			setContentForPHP() 			&&
		#endif
		startDataTransmition(dataLength))
	{
		refTools.state.http.isBussy = true;
		return true;
	}
	
	return false;
}


bool HTTPHandler::initGetRequest(IPAddress& address, const char* url){
	if(initSession()){
		setGetURL(address, url);
		refTools.state.http.isBussy = true;
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
	if(refTools.state.http.isBussy){
		return false;
	}
	
	refTools.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	ANWSER_CODES code = refTools.readAndGetCode();
	switch (code){
		case OK:
			return true;
		
		case ERROR:
		case UNDEFINED:
			if(terminateSession()){
				refTools.writeHTPP(HTTP_INIT);
				return refTools.readAndExpectSuccess();
			}else{
				return false;
			}
			
		default: break;
	}
	
	return false;
}


bool HTTPHandler::setPostURL(IPAddress& address, const char* url){
	refTools.writeHTTPURL(address, url);
	return refTools.readAndExpectSuccess();
}


void HTTPHandler::setGetURL(IPAddress& address, const char* url){
	refTools.writeHTTPURL(address, url, false);
}


bool HTTPHandler::setContentForPHP(){
	refTools.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	return refTools.readAndExpectSuccess();
}


bool HTTPHandler::startDataTransmition(int dataLength){
	refTools.writeHTPPData(dataLength);
	if(!refTools.readTimeout(LONG_WAIT)){
		return false;
	}
	
	if(!refTools.containDownload()){
		if(!refTools.readTimeout(LONG_WAIT)){
			return false;
		}
		if(!refTools.containDownload()){
			return false;
		}
	}
	
	return true;
}



bool HTTPHandler::terminateSession(){
	refTools.writeHTPP(HTTP_TERM);
	return refTools.readAndExpectSuccess();
}