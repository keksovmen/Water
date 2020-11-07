#include <Arduino.h>
#include "SimHandlerHelper.h"



template<int N>
SimHandlerHelper<N>::SimHandlerHelper(Stream& connection, ParameterHandler& parameters) :
	handler(connection, buffer, parameters),
	refParameters(parameters)
{

}


template<int N>
bool SimHandlerHelper<N>::sendParams(ParameterHandler& params){
	dataHandler = handler.sendPostRequest(
			params.getAddress().getValue(),
			"Send.php", 
			params.getLength()
		);
	
	if(!dataHandler){
		return false;
	}
	
	params.handleWritingValue(*dataHandler);
	lastRequest = HTTP_SCRIPT_SEND_PARAMS;
	
	
	return handleSendRootine();
}


//TODO: finish it made volume as local param with id and so on
template<int N>
bool SimHandlerHelper<N>::sendVolume(
		// Parameter<PrimitivIntParameter<int>>& volume, 
		ParameterHandler& params
		)
{
	int length = params.getGivenVolume().getLength() + 
					params.getClock().getLength() +
					params.getCard().getLength() + 2;
	
	dataHandler = handler.sendPostRequest(
			params.getAddress().getValue(),
			"SendVolume.php", 
			length
		);
	
	if(!dataHandler){
		return false;
	}
	
	params.getClock().handleWritingValue(*dataHandler);
	dataHandler->write('&');
	params.getGivenVolume().handleWritingValue(*dataHandler);
	dataHandler->write('&');
	params.getCard().handleWritingValue(*dataHandler);
	
	lastRequest = HTTP_SCRIPT_SEND_VOLUME;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::askVolume(){
	dataHandler = handler.sendGetRequest(
			refParameters.getAddress().getValue(),
			"/GetVolume.php"
			);
	
	if(!dataHandler){
		return false;
	}
	
	lastRequest = HTTP_SCRIPT_GET_VOLUME;
	
	
	return handleSendRootine();
}


template<int N>
bool SimHandlerHelper<N>::askTime(){
	dataHandler = handler.sendGetRequest(
			refParameters.getAddress().getValue(),
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
bool SimHandlerHelper<N>::isAnwserRdy(){
	bool result = dataHandler->isSended();
	if(!result){
		handler.doActivity();
	}
	
	return result;
}


template<int N>
bool SimHandlerHelper<N>::isAnwserSuccess(){
	bool result = dataHandler->isSendedSuccesfully();
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
		int index = b.indexOf("#");
		if(index == -1){
			continue;
		}
		
		int end = b.indexOfFrom(index, "$");
		if(end == -1){
			continue;
		}
		
		refParameters.getClock().getValue().parse(&b[index + 1]);
		break;
	}
}


template<int N>
void SimHandlerHelper<N>::parseVolume(){
	dataHandler->getBuffer().clear();
	
	while(dataHandler->readResponce()){
		auto& b = dataHandler->getBuffer();
		int index = b.indexOf("#");
		if(index == -1){
			continue;
		}
		
		int end = b.indexOfFrom(index, "$");
		if(end == -1){
			continue;
		}
		
		refParameters.
			getUserVolume().
					getValue().
						getValue() = atoi(&b[index + 1]);
		break;
	}
}