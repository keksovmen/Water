#include <Arduino.h>
#include "SimFacade.h"
#include "Util.h"


template<int N>
SimFacade<N>::SimFacade(SoftwareSerial& refPort) :
	wrapper(refPort), writer(wrapper),
	parser(wrapper.getBuffer()), gprsHandler(wrapper, writer, parser)
{
	
}

template<int N>
bool SimFacade<N>::isModuleUp(){
	writer.writeAT();
	
	if(!wrapper.readToBuffer()){
		//if minimum time has passed and there is still no anwser
		return false;
	}
	
	if(parser.isSimpleMessageReady()){
		ANWSER_CODES numberFetch = static_cast<ANWSER_CODES>(
									parser.fetchResultCode()
									);
			
		//if code is undefiend try text variant
		if(numberFetch == ANWSER_CODES::UNDEFINED){
			
			numberFetch = static_cast<ANWSER_CODES>(
						parser.fetchSimpleTextCode()
						);
		}
			
		return numberFetch == ANWSER_CODES::OK;
	}else{
		//TODO: Coud fail here due to Unsolicited messages
		//UNDER VOLTAG etc. fix it, through parser
		return isModuleUp();
	}
	
	return false;
}


template<int N>
NETWORK_CONNECTION SimFacade<N>::isConnectedToNetwork(){
	writer.writeCREG();
	
	if(!wrapper.readToBuffer()){
		//if minimum time has passed and there is still no anwser
		return NETWORK_CONNECTION::UNKNOWN;
	}
	
	if(parser.isComplexMessageReady()){
		if(parser.fetchResultCode() ==
				ANWSER_CODES::OK){
			return static_cast<NETWORK_CONNECTION>(
					parser.fetchNetworkRegistration()
					);
		}else{
			return NETWORK_CONNECTION::UNKNOWN;
		}
	}else{
		return isConnectedToNetwork();
	}
	
	return NETWORK_CONNECTION::UNKNOWN;
}


template<int N>
bool SimFacade<N>::setDefaultParams(){
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
bool SimFacade<N>::connectToGPRS(const char* apn){
	if(gprsHandler.isConnected()){
		return true;
	}
	
	return gprsHandler.connect(apn);
}

template<int N>
bool SimFacade<N>::disconnectFromGPRS(){
	if(!gprsHandler.isConnected()){
		return true;
	}
	
	return gprsHandler.close();
}


template<int N>
PostDataHandler<N> SimFacade<N>::sendPostRequest(const char* url, int dataLength){
	writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
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
GetDataHandler<N> SimFacade<N>::sendGetRequest(){
	writer.writeHTPP(HTTP_COMMANDS::HTTP_INIT);
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam("URL", nullptr);
	
	return GetDataHandler<N>(wrapper, parser, writer);
}