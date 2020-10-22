#include <Arduino.h>
#include <New.h>
#include "SimHandler.h"
#include "Util.h"
#include "HTTPHandler.h"
#include "PostDataHandler.h"
#include "GetDataHandler.h"


//For new Post/GetDataHandler to return a pointer
//Caution first check or calculate sizeof(Post/GetDataHandler)
static char dynamicMemory[19];



template<int N>
SimHandler<N>::SimHandler(Stream& refPort) :
	wrapper(refPort), writer(wrapper),
		parser(wrapper.getBuffer()), 
		reader(wrapper, parser, writer),
		gprsHandler(reader, writer, parser), 
		httpHandler(reader, writer, parser)
{
	
}

template<int N>
bool SimHandler<N>::isModuleUp(){
	writer.writeAT();
	
	bool result = readAndExpectSuccess(reader, parser);
	wrapper.getBuffer().clear();
	
	return result;
}


template<int N>
bool SimHandler<N>::isModuleAlive(){
	writer.writeCPIN();
	
	bool result = readAndExpectSuccess(reader, parser);
	if(result){
		result = parser.isPinRdy();
	}
	
	wrapper.getBuffer().clear();
	
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
	writer.writeCREG();
	
	NETWORK_CONNECTION status = NETWORK_CONNECTION::UNKNOWN;
	if(readAndExpectSuccess(reader, parser, true)){
		//if minimum time has passed and there is still no anwser
		status = static_cast<NETWORK_CONNECTION>(parser.fetchNetworkRegistration());
	}
	
	wrapper.getBuffer().clear();
	
	return status;
}


template<int N>
bool SimHandler<N>::setDefaultParams(){
	writer.writeEcho(false);
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	writer.writeNumberFormat(false);
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	parser.setState(PARSER_STATE_TEXT);
	
	writer.writeCallReady(false);
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	writer.writeReportAsError(true);
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	writer.writeIPR(115200);
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	writer.writeAT();
	if(!readAndExpectSuccess(reader, parser)){
		wrapper.getBuffer().clear();
		return false;
	}
	
	wrapper.getBuffer().clear();
	
	return true;
}


template<int N>
bool SimHandler<N>::connectToGPRS(const char* apn){
	bool result = gprsHandler.isConnected();
	if(!result){
		result = gprsHandler.connect(apn);
	}
	
	wrapper.getBuffer().clear();
	
	return result;
}

template<int N>
bool SimHandler<N>::disconnectFromGPRS(){
	bool result = gprsHandler.isConnected();
	if(result){
		result = gprsHandler.close();
	}
	
	wrapper.getBuffer().clear();
	
	return result;
}


template<int N>
DataHandler<N>* SimHandler<N>::sendPostRequest(IPAddress& address, const char* url, int dataLength){
	if(httpHandler.initPostRequest(address, url, dataLength)){
		wrapper.getBuffer().clear();
		return new(dynamicMemory) PostDataHandler<N>(wrapper, parser, writer, 
													reader, wrapper.getBuffer());
	}
	
	wrapper.getBuffer().clear();
	
	return nullptr;
}


template<int N>
DataHandler<N>* SimHandler<N>::sendGetRequest(){
	if(httpHandler.initGetRequest()){
		wrapper.getBuffer().clear();
		return new(dynamicMemory) GetDataHandler<N>(wrapper, parser, writer,
													reader, wrapper.getBuffer());
	}
	
	wrapper.getBuffer().clear();
	
	return nullptr;
}


template<int N>
void SimHandler<N>::handleReading(){
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
	wrapper.getBuffer().clear();
}