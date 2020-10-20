#include "HTTPHandler.h"
#include "Util.h"
#include "Enums.h"


template<int N>
HTTPHandler<N>::HTTPHandler(BaseReader& reader, 
			SimCommandWriter& writer, 
			SimResultParser<N>& parser) :
	refReader(reader), refWriter(writer), refParser(parser)
{
	
	
}


template<int N>
bool HTTPHandler<N>::initPostRequest(IPAddress& address, const char* url, int dataLength){
	if(initSession() 		&&
		setPostURL(address, url) 	&&
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
	refWriter.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	ANWSER_CODES code = readAndGetCode(refReader, refParser);
	switch (code){
		case OK:
			return true;
		
		case ERROR:
		case UNDEFINED:
			if(terminateSession()){
				refWriter.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
				return readAndExpectSuccess(refReader, refParser);
			}else{
				return false;
			}
			
		default: break;
	}
	
	return false;
}

template<int N>
bool HTTPHandler<N>::setPostURL(IPAddress& address, const char* url){
	refWriter.writeHTTPURL(address, url);
	return readAndExpectSuccess(refReader, refParser);
}

template<int N>
void HTTPHandler<N>::setGetURL(){
	refWriter.writeHTPPSetParam("URL", nullptr);
}

template<int N>
bool HTTPHandler<N>::setContentForPHP(){
	refWriter.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	return readAndExpectSuccess(refReader, refParser);
}

template<int N>
bool HTTPHandler<N>::startDataTransmition(int dataLength){
	refWriter.writeHTPPData(dataLength);
	if(!refReader.read()){
		return false;
	}
	
	if(!refParser.containDownload()){
		if(!refReader.read()){
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
	refWriter.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	return readAndExpectSuccess(refReader, refParser);
}