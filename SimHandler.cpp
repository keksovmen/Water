#include <Arduino.h>
#include <New.h>
#include "SimHandler.h"
#include "Util.h"
#include "HTTPHandler.h"
#include "PostDataHandler.h"
#include "GetDataHandler.h"


//For new Post/GetDataHandler to return a pointer
//Caution first check or calculate sizeof(Post/GetDataHandler)
static char dynamicMemory[15];



template<int N>
SimHandler<N>::SimHandler(
				Stream& refPort, 
				ParameterHandler& parameters
				) :
		wrapper(refPort, buffer), 
		reader(buffer, wrapper),
		parser(buffer), 
		simPort(wrapper, reader),
		tcpHandler(simPort, parser, parameters),
		gprsHandler(simPort, parser), 
		httpHandler(simPort, parser)
{
	reader.attachTCPHandler(&tcpHandler);
}

template<int N>
bool SimHandler<N>::isModuleUp(){
	simPort.writeAT();
	
	bool result = readAndExpectSuccess(reader, parser);
	buffer.clear();
	
	return result;
}


template<int N>
bool SimHandler<N>::isModuleAlive(){
	simPort.writeCPIN();
	
	bool result = readAndExpectSuccess(reader, parser);
	if(result){
		result = parser.isPinRdy();
	}
	
	buffer.clear();
	
	return result;
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
	simPort.writeCREG();
	
	NETWORK_CONNECTION status = NETWORK_CONNECTION::UNKNOWN;
	if(readAndExpectSuccess(reader, parser, true)){
		//if minimum time has passed and there is still no anwser
		status = static_cast<NETWORK_CONNECTION>(
					parser.fetchNetworkRegistration()
					);
	}
	
	buffer.clear();
	
	return status;
}


template<int N>
bool SimHandler<N>::setDefaultParams(){
	if(!tryToSetDefaultParam(0)){
		return false;
	}
	
	if(!tryToSetDefaultParam(1)){
		return false;
	}
	parser.setState(PARSER_STATE_TEXT);
	
	if(!tryToSetDefaultParam(2)){
		return false;
	}
	
	if(!tryToSetDefaultParam(3)){
		return false;
	}
	
	if(!tryToSetDefaultParam(4)){
		return false;
	}
	
	
	return true;
}


template<int N>
bool SimHandler<N>::connectToGPRS(const char* apn){
	bool result = gprsHandler.isConnected();
	if(!result){
		result = gprsHandler.connect(apn);
	}
	
	buffer.clear();
	
	return result;
}

template<int N>
bool SimHandler<N>::disconnectFromGPRS(){
	bool result = gprsHandler.isConnected();
	if(result){
		result = gprsHandler.close();
	}
	
	buffer.clear();
	
	return result;
}


template<int N>
DataHandler<N>* SimHandler<N>::sendPostRequest(
							IPAddress& address, 
							const char* url, 
							int dataLength)
{
	if(httpHandler.initPostRequest(address, url, dataLength)){
		buffer.clear();
		return new(dynamicMemory) 
				PostDataHandler<N>(parser, simPort, buffer);
	}
	
	buffer.clear();
	
	return nullptr;
}


template<int N>
DataHandler<N>* SimHandler<N>::sendGetRequest(){
	if(httpHandler.initGetRequest()){
		buffer.clear();
		return new(dynamicMemory)
				GetDataHandler<N>(parser, simPort, buffer);
	}
	
	buffer.clear();
	
	return nullptr;
}


template<int N>
void SimHandler<N>::handleReading(){
	if(!tcpHandler.isConnected()){
		tcpHandler.connect();
	}
	
	if(!wrapper.lazyRead()){
		return;
	}
	
	reader.handleSwitch();
	
	//if will contain a message try to read and parse
	//through UnexpectedHandler
	if(parser.isPossibleMessage()){
		reader.read();
	}
	
	//clear possible garbage
	buffer.clear();
}


template<int N>
void SimHandler<N>::writeDefaultParam(int id){
	switch(id){
		case 0:
			simPort.writeEcho(false);
			break;
		case 1:
			simPort.writeNumberFormat(false);
			break;
		case 2:
			simPort.writeCallReady(false);
			break;
		case 3:
			simPort.writeReportAsError(true);
			break;
		case 4:
			simPort.writeIPR(115200);
			break;
		
		default : Serial.println("MISSID ID");
	}
}


template<int N>
bool SimHandler<N>::tryToSetDefaultParam(int id){
	bool result = true;
	
	writeDefaultParam(id);
	if(readAndGetCode(reader, parser) == ANWSER_CODES::UNDEFINED){
		writeDefaultParam(id);
		result = readAndExpectSuccess(reader, parser);
	}
	
	
	buffer.clear();
	return result;
}