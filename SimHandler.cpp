#include <Arduino.h>
#include "SimHandler.h"
#include "Util.h"
#include "HTTPHandler.h"


template<int N>
SimHandler<N>::SimHandler(SoftwareSerial& refPort) :
	wrapper(refPort), writer(wrapper),
	parser(wrapper.getBuffer()), gprsHandler(wrapper, writer, parser)
{
	
}

template<int N>
bool SimHandler<N>::isModuleUp(){
	writer.writeAT();
	
	if(!readAndExpectSuccess(wrapper, parser)){
		return false;
	}
	
	return true;
}


/**
	After you send AT+CREG? with AT+CREG=0
	you wull get result as +CREG: n,s
		where:
		n = current setting of representation
			0 - if network changes there won't be a notification
			1 - if network changes there is notification
			2 - same as 1 plus response to this command is additional
				info about location area code, and cell id as hex bytes
		s = current status of network
			0 - not registered, and not even trying to
			1 - registered home network
			2 - trying to find a network
			3 - registr denied
			4 - Unkown
			5 - registered, romaning
*/

template<int N>
NETWORK_CONNECTION SimHandler<N>::isConnectedToNetwork(){
	writer.writeCREG();
	if(!readAndExpectSuccess(wrapper, parser, true)){
		//if minimum time has passed and there is still no anwser
		return NETWORK_CONNECTION::UNKNOWN;
	}
	
	return static_cast<NETWORK_CONNECTION>(parser.fetchNetworkRegistration());
}


template<int N>
bool SimHandler<N>::setDefaultParams(){
	writer.writeEcho(false);
	if(!readAndExpectSuccess(wrapper, parser))
		return false;
	
	writer.writeNumberFormat(true);
	if(!readAndExpectSuccess(wrapper, parser))
		return false;
	
	writer.writeCallReady(false);
	if(!readAndExpectSuccess(wrapper, parser))
		return false;
	
	writer.writeReportAsError(true);
	if(!readAndExpectSuccess(wrapper, parser))
		return false;
	
	writer.writeAT();
	if(!readAndExpectSuccess(wrapper, parser))
		return false;
	
	return true;
}


template<int N>
bool SimHandler<N>::connectToGPRS(const char* apn){
	if(gprsHandler.isConnected()){
		return true;
	}
	
	return gprsHandler.connect(apn);
}

template<int N>
bool SimHandler<N>::disconnectFromGPRS(){
	if(!gprsHandler.isConnected()){
		return true;
	}
	
	return gprsHandler.close();
}


template<int N>
PostDataHandler<N> SimHandler<N>::sendPostRequest(const char* url, int dataLength){
	writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	//Add check if already init so you could close it
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam("URL", url);
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam("CONTENT", "application/x-www-form-urlencoded");
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPData(dataLength);
	if(!wrapper.readToBuffer()){
		Serial.println("Error with sendPostRequest() 1");
		//error
	}
	
	//error
	if(!parser.containDownload()){
		if(!wrapper.readToBuffer()){
			Serial.println("Error with sendPostRequest() 2");
		}
		if(!parser.containDownload()){
			Serial.println("Error with sendPostRequest() 3");
		}
	}
	
	return PostDataHandler<N>(wrapper, parser, writer);
}


template<int N>
GetDataHandler<N> SimHandler<N>::sendGetRequest(){
	writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	//Add check if already init so you could close it
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam("URL", nullptr);
	
	return GetDataHandler<N>(wrapper, parser, writer);
}