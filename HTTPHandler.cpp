#include "HTTPHandler.h"
#include "Util.h"
#include "Enums.h"


template<int N>
HTTPHandler<N>::HTTPHandler(SimIOWrapper<N>& refWrapper, 
			SimCommandWriter<N>& refWriter, 
			SimResultParser<N>& refParser) :
	wrapper(refWrapper), writer(refWriter), parser(refParser)
{
	
	
}


template<int N>
bool HTTPHandler<N>::initPostRequest(const char* url, int dataLength){
	if(initSession() 		&&
		setPostURL(url) 	&&
		setContentForPHP() 	&&
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
	writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	ANWSER_CODES code = readAndGetCode(wrapper, parser);
	switch (code){
		case OK:
			return true;
		
		case ERROR:
			if(terminateSession()){
				writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
				return readAndExpectSuccess(wrapper, parser);
			}else{
				return false;
			}
			
		default: break;
	}
	
	return false;
}

template<int N>
bool HTTPHandler<N>::setPostURL(const char* url){
	writer.writeHTPPSetParam("URL", url);
	return readAndExpectSuccess(wrapper, parser);
}

template<int N>
void HTTPHandler<N>::setGetURL(){
	writer.writeHTPPSetParam("URL", nullptr);
}

template<int N>
bool HTTPHandler<N>::setContentForPHP(){
	writer.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	return readAndExpectSuccess(wrapper, parser);
}

template<int N>
bool HTTPHandler<N>::startDataTransmition(int dataLength){
	writer.writeHTPPData(dataLength);
	if(!wrapper.readToBuffer()){
		return false;
	}
	
	if(!parser.containDownload()){
		if(!wrapper.readToBuffer()){
			return false;
		}
		if(!parser.containDownload()){
			return false;
		}
	}
	return true;
}


template<int N>
bool HTTPHandler<N>::terminateSession(){
	writer.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	return readAndExpectSuccess(wrapper, parser);
}