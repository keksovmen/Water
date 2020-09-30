#include <Arduino.h>
#include "SimFacade.h"
#include "Util.h"


template<int N>
SimFacade<N>::SimFacade(SoftwareSerial& refPort) :
wrapper(refPort), gprsHandler(wrapper, writer, parser)
{
	
}

template<int N>
bool SimFacade<N>::isModuleUp(){
	if(!writer.writeAT(wrapper)){
		//if there is problem then next command will be corrapted
		//TODO: fix it, maybe remember last command and how much 
		//remains to write, or just send couple AT command to clear buffer
		return false;
	}
	
	if(!wrapper.readToBuffer()){
		//if minimum time has passed and there is still no anwser
		return false;
	}
	
	if(parser.isSimpleMessageReady(wrapper.getBuffer())){
		ANWSER_CODES numberFetch = static_cast<ANWSER_CODES>(
			parser.fetchResultCode(wrapper.getBuffer()));
		switch (numberFetch){
			case OK:
				return true;
				
			case UNDEFINED:
				numberFetch = static_cast<ANWSER_CODES>(
					parser.fetchSimpleTextCode(wrapper.getBuffer()));
				if(numberFetch == ANWSER_CODES::OK){
					return true;
				}
				return false;
				
			default:
				return false;
		}
	}else{
		//TODO: Coud fail here due to Unsolicited messages
		//UNDER VOLTAG etc. fix it, through parser
		return isModuleUp();
	}
	
	return false;
}


template<int N>
NETWORK_CONNECTION SimFacade<N>::isConnectedToNetwork(){
	if(!writer.writeCREG(wrapper)){
		//if there is problem then next command will be corrapted
		//TODO: fix it, maybe remember last command and how much 
		//remains to write, or just send couple AT command to clear buffer
		return NETWORK_CONNECTION::UNKNOWN;
	}
	
	if(!wrapper.readToBuffer()){
		//if minimum time has passed and there is still no anwser
		return NETWORK_CONNECTION::UNKNOWN;
	}
	
	if(parser.isComplexMessageReady(wrapper.getBuffer())){
		if(parser.fetchResultCode(wrapper.getBuffer()) ==
				ANWSER_CODES::OK){
			return static_cast<NETWORK_CONNECTION>(
				parser.fetchNetworkRegistration(wrapper.getBuffer()));
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
	if(writer.writeEcho(wrapper, false) &&
			writer.writeNumberFormat(wrapper, true) &&
			writer.writeCallReady(wrapper, false) &&
			writer.writeReportAsError(wrapper, true) &&
			writer.writeAT(wrapper)){
		if(wrapper.readToBuffer()){
			if(parser.fetchResultCode(wrapper.getBuffer()) == ANWSER_CODES::OK){
				return true;
			}
		}
	}
	return false;
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
	writer.writeHTPP(wrapper, HTTP_COMMANDS::HTTP_INIT);
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam(wrapper, "URL", url);
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPSetParam(wrapper, "CONTENT", "application/x-www-form-urlencoded");
	readAndExpectSuccess(wrapper, parser);
	
	writer.writeHTPPData(wrapper, dataLength);
	if(!wrapper.readToBuffer()){
		Serial.println("Error with sendPostRequest() 1");
		//error
	}
	
	if(!parser.containDownload(wrapper.getBuffer())){
		// delay(50);
		if(!wrapper.readToBuffer()){
			Serial.println("Error with sendPostRequest() 3");
		}
		if(!parser.containDownload(wrapper.getBuffer())){
			Serial.println("Error with sendPostRequest() 2");
		}
		//error
	}
	
	return PostDataHandler<N>(wrapper, parser, writer);
}