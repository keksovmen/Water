#include <Arduino.h>
#include "SimHandlerHelper.h"



template<int N>
SimHandlerHelper<N>::SimHandlerHelper(Stream& connection) :
	handler(connection){
	
}

template<int N>
bool SimHandlerHelper<N>::init(){
	if(isInit)
		return true;
	
	if(!handler.isModuleUp()){
		Serial.println("Module is offline");
		return false;
	}
	
	
	if(!handler.setDefaultParams()){
		Serial.println("Defaults are not set");
		return false;
	}
	
	//dosn't retun false because technically module is working
	//and default are set so initialized successfully
	//here just for debug purposes mainly
	//prevent early than too late
	if(!handler.isModuleAlive()){
		Serial.println("Sim is not inserted");
	}
	
	isInit = true;
	return true;
}

template<int N>
bool SimHandlerHelper<N>::sendParams(ParameterHandler& params){
	if(!initRotine()){
		return false;
	}
	
	if(!checkReadyToSend()){
		return false;
	}
	
	
	auto* dataHandler = handler.sendPostRequest(
			params.getAddress().getValue(),
			"Send.php", 
			params.getLength()
		);
	
	if(!dataHandler){
		return false;
	}
	
	
	params.handleWritingValue(*dataHandler);
	
	bool result = false;
	if(handleSendRootine(dataHandler)){
		result = true;
	}
	
	dataHandler->finish();
	
	return result;
}


//TODO: finish it made volume as local param with id and so on
template<int N>
bool SimHandlerHelper<N>::sendVolume(int volume){
	if(!initRotine()){
		return false;
	}
	
	if(!checkReadyToSend()){
		return false;
	}
	
	
	// auto* dataHandler = handler.sendPostRequest(
			// "http://128.69.240.186/SendVolume.php", 
			// 10
		// );
	
	
	// if(!dataHandler){
		// return false;
	// }
	
	
	// params.handleWritingValue(*dataHandler);
	
	bool result = false;
	// if(handleSendRootine(dataHandler)){
		// result = true;
	// }
	
	// dataHandler->finish();
	
	return result;
}








template<int N>
bool SimHandlerHelper<N>::initRotine(){
	if(!isInit)
		init();
	
	return isInit;
}

template<int N>
bool SimHandlerHelper<N>::checkReadyToSend(){
	if(!handler.isModuleAlive()){
		Serial.println("Sim is not inserted");
		return false;
	}
	
	if(!handleNetworkStatus(handler.isConnectedToNetwork())){
		Serial.println("Network is not registered");
		return false;
	}
	
	if(!handler.connectToGPRS("internet")){
		Serial.println("GPRS is not connected");
		return false;
	}
	
	return true;
}

template<int N>
bool SimHandlerHelper<N>::handleNetworkStatus(NETWORK_CONNECTION status){
	switch(status){
		case REGISTERED:
			return true;
			
			
		case NOT_REGISTERED_SEARCHING:
				//wait for 3 sec and check if registered
				for(int i = 0; i < 10; i++){
					delay(300);
					if(handler.isConnectedToNetwork() 
							== REGISTERED)
						return true;
				}
				
			return false;
			
			
		default : return false;
	}
}


template<int N>
bool SimHandlerHelper<N>::handleSendRootine(DataHandler<N>* dataHandler){
	if(!dataHandler->send()){
		Serial.println("Send failed");
		return false;
	}
	
	while(!dataHandler->isSended()){
		//just wait or could add some interapt
		//so you could break from here and
		//somehow later continue from here
		delay(100);
	}
	
	if(!dataHandler->isSendedSuccesfully()){
		Serial.println("Sended with unsuccess code");
		return false;
	}
	
	return true;
}