#include <Arduino.h>
#include "SimHandlerHelper.h"
#include "ParameterWriter.h"
#include "Constants.h"
#include "ParameterHandler.h"



template<int N>
SimHandlerHelper<N>::SimHandlerHelper(Stream& connection) :
	handler(connection, buffer){}


template<int N>
bool SimHandlerHelper<N>::sendParams(){
	ParameterWriter paramWriter;
	ParameterHandler& refParameters = ParameterHandler::getInstance();
	paramWriter.add(&refParameters.getClock());
	paramWriter.add(&refParameters.getSensorTempUp());
	paramWriter.add(&refParameters.getSensorTempDown());
	paramWriter.add(&refParameters.getPlateId());
	
	dataHandler = handler.sendPostRequest(
			refParameters.getAddress().getValue(),
			"Send.php", 
			paramWriter.getLength()
		);
	
	if(!dataHandler){
		return false;
	}
	
	paramWriter.handleWritingValue(*dataHandler);
	lastRequest = HTTP_SCRIPT_SEND_PARAMS;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::sendVolume()
{
	ParameterWriter paramWriter;
	ParameterHandler& refParameters = ParameterHandler::getInstance();
	paramWriter.add(&refParameters.getGivenVolume());
	paramWriter.add(&refParameters.getClock());
	paramWriter.add(&refParameters.getCard());
	
	dataHandler = handler.sendPostRequest(
			refParameters.getAddress().getValue(),
			"SendVolume.php", 
			paramWriter.getLength()
		);
	
	if(!dataHandler){
		return false;
	}
	
	paramWriter.handleWritingValue(*dataHandler);
	lastRequest = HTTP_SCRIPT_SEND_VOLUME;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::askVolume(){
	ParameterHandler& refParameters = ParameterHandler::getInstance();
	dataHandler = handler.sendGetRequest(
			refParameters.getAddress().getValue(),
			"/GetVolume.php"
			);
	
	if(!dataHandler){
		return false;
	}
	
	dataHandler->write('?');
	refParameters.getCard().handleWritingValue(*dataHandler);
	lastRequest = HTTP_SCRIPT_GET_VOLUME;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::askTime(){
	dataHandler = handler.sendGetRequest(
			ParameterHandler::getInstance().getAddress().getValue(),
			"/GetTime.php"
			);
	
	if(!dataHandler){
		return false;
	}
	
	//write your query string
	// dataHandler->write("?a=1&b=2");
	
	lastRequest = HTTP_SCRIPT_GET_TIME;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::isAbleToUseHttp(){
	return handler.tools.state.isAbleToSendHttp();
}


template<int N>
int SimHandlerHelper<N>::isAnwserRdy(){
	if(!handler.tools.state.isMinimumEstablished()){
		return -1;
	}
	
	bool result = dataHandler->isSended();
	if(!result){
		handler.doActivity();
	}
	
	return result ? 1 : 0;
}


template<int N>
bool SimHandlerHelper<N>::isAnwserSuccess(){
	bool result = dataHandler->isSendedSuccesfully();
	if(!handler.tools.state.isMinimumEstablished()){
		result = false;
	}
	
	if(result){
		//handle what was send here
		switch(lastRequest){
			case HTTP_SCRIPT_GET_TIME:
				parseTime();
				break;
				
			case HTTP_SCRIPT_GET_VOLUME:
				parseVolume();
				break;
				
			case HTTP_SCRIPT_SEND_PARAMS:
			case HTTP_SCRIPT_SEND_VOLUME:
				
				break;
				
			default: break;
		}
	}
	
	finishSession();
	
	return result;
}


template<int N>
void SimHandlerHelper<N>::abort(){
	finishSession();
	handler.gprsHandler.close();
	// finishSession();
}

template<int N>
bool SimHandlerHelper<N>::handleSendRootine(){
	if(!dataHandler->send()){
		Serial.println("Send failed");
		finishSession();
		return false;
	}
	
	// this->dataHandler = dataHandler;
	
	return true;
}


template<int N>
void SimHandlerHelper<N>::finishSession(){
	lastRequest = HTTP_SCRIPT_NAN;
	if(dataHandler){
		dataHandler->finish();
	}
	
	dataHandler = nullptr;
}


template<int N>
void SimHandlerHelper<N>::parseTime(){
	dataHandler->getBuffer().clear();
	
	while(dataHandler->readResponce()){
		auto& b = dataHandler->getBuffer();
		int index = b.indexOf(ENTRY_BEGINING);
		if(index == -1){
			continue;
		}
		
		int end = b.indexOfFrom(index, ENTRY_ENDING);
		if(end == -1){
			continue;
		}
		
		ParameterHandler::getInstance().getClock().getValue().parse(&b[index + 1]);
		break;
	}
}


template<int N>
void SimHandlerHelper<N>::parseVolume(){
	dataHandler->getBuffer().clear();
	
	while(dataHandler->readResponce()){
		auto& b = dataHandler->getBuffer();
		int index = b.indexOf(ENTRY_BEGINING);
		if(index == -1){
			continue;
		}
		
		int end = b.indexOfFrom(index, ENTRY_ENDING);
		if(end == -1){
			continue;
		}
		
		ParameterHandler::getInstance().
			getUserVolume().
					getValue().
						getValue() = atoi(&b[index + 1]);
		break;
	}
}