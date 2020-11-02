#include <Arduino.h>
#include <New.h>
#include "SimHandler.h"
#include "Util.h"
#include "HTTPHandler.h"
#include "PostDataHandler.h"
#include "GetDataHandler.h"
#include "TCPIncomingHandler.h"


//For new Post/GetDataHandler to return a pointer
//Caution first check or calculate sizeof(Post/GetDataHandler)
static char dynamicMemory[21];



template<int N>
SimHandler<N>::SimHandler(
				Stream& refPort, 
				ParameterHandler& parameters
				) :
		wrapper(refPort, buffer), 
		reader(buffer, wrapper, tools.state),
		tools(wrapper, reader, buffer),
		gprsHandler(tools), 
		cgattHandler(tools),
		httpHandler(tools),
		tcpHandler(tools, parameters),
		refParams(parameters)
{
	reader.attachTCPHandler(&tcpHandler);
}

template<int N>
bool SimHandler<N>::isModuleUp(){
	tools.simPort.writeAT();
	
	bool result = tools.readAndExpectSuccess();
	buffer.clear();
	
	return result;
}


template<int N>
bool SimHandler<N>::isModuleAlive(){
	tools.simPort.writeCPIN();
	
	bool result = tools.readAndExpectSuccess();
	if(result){
		result = tools.parser.isPinRdy();
	}else{
		//if sim currently bussy doing initialisation
		if(tools.parser.getLastError() == CME_ERROR_SIM_BUSSY){
			tools.state.timer.sheduleDelay(5000);
		}
	}
	
	
	buffer.clear();
	
	tools.state.health.cpin = result;
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
	tools.simPort.writeCREG();
	
	NETWORK_CONNECTION status = NETWORK_CONNECTION::UNKNOWN;
	if(tools.readAndExpectSuccess()){
		//if minimum time has passed and there is still no anwser
		status = static_cast<NETWORK_CONNECTION>(
					tools.parser.fetchNetworkRegistration()
					);
	}
	
	buffer.clear();
	tools.state.health.networkRegistration = (status == REGISTERED);
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
	tools.parser.setState(PARSER_STATE_TEXT);
	
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
	bool result = gprsHandler.isConnected() == GPRS_CONNECTED;
	if(!result){
		result = gprsHandler.connect(apn);
	}
	
	buffer.clear();
	
	return result;
}

template<int N>
bool SimHandler<N>::disconnectFromGPRS(){
	bool result = gprsHandler.isConnected() == GPRS_CONNECTED;
	// if(result){
		// result = gprsHandler.close();
	// }
	
	// buffer.clear();
	
	return result;
}


template<int N>
DataHandler* SimHandler<N>::sendPostRequest(
							IPAddress& address, 
							const char* url, 
							int dataLength)
{
	if(httpHandler.initPostRequest(address, url, dataLength)){
		buffer.clear();
		return new(dynamicMemory) 
				PostDataHandler(tools.parser, tools.simPort, buffer, tools.state);
	}
	
	buffer.clear();
	
	return nullptr;
}


template<int N>
DataHandler* SimHandler<N>::sendGetRequest(
							IPAddress& address, 
							const char* url
							){
	if(httpHandler.initGetRequest(address, url)){
		buffer.clear();
		return new(dynamicMemory)
				GetDataHandler(tools.parser, tools.simPort, buffer, tools.state);
	}
	
	buffer.clear();
	
	return nullptr;
}


template<int N>
void SimHandler<N>::handleReading(){
}


//TODO: Cut into bool functions big blocks of handling
template<int N>
void SimHandler<N>::doActivity(){
	if(!tools.state.timer.isOpen()){
		goto B;
	}
	
	if(handleLongMessages()){
		return;	//currently waiting for anwser
	}
	
	if(!tools.state.health.defaultsAreSet){
		if(!isModuleUp()){
			return;
		}
		
		if(setDefaultParams()){
			tools.state.health.defaultsAreSet = true;
		}else{
			return;
		}
	}
	
	if(!tools.state.isMinimumEstablished()){
		if(!tools.state.health.cpin && !isModuleAlive()){
			//skip turns and wait until sim will be ok
			return;
		}
		
		if(isConnectedToNetwork() != 
				NETWORK_CONNECTION::REGISTERED){
			//skip turns and wait until network will be ok
			return;
		}
	}
	
	if(!tools.state.health.CGATT_Connection){
		if(!cgattHandler.connectToCGATT()){
			return;
		}
	}
	
	if(!tools.state.isGPRS_Connected()){
		A:
		switch(tools.state.health.GPRS_Connection){
			case GPRS_UNDEFINIED:
				if(gprsHandler.isConnected() != GPRS_UNDEFINIED){
					goto A;
				}
				return;	//wait until module is send something
				
			case GPRS_CLOSED:
				//take APN from params
				gprsHandler.connect("internet");
				return;
				
			default: return;	//wait for result
		}
	}
	
	if(!tools.state.isTCP_Connected()){
		if(tcpHandler.connect()){
			return;
		}
	}
	
	if(tools.state.tcp.hasMessage){
		handleTCPMessage();
		return;
	}
	
	
	B:
	if(!wrapper.lazyRead()){
		return;
	}
	
	reader.handleSwitch();
	
	// if will contain a message try to read and parse
	// through UnexpectedHandler
	if(tools.parser.isPossibleMessage()){
		reader.read();
	}
	
	//clear possible garbage
	buffer.clear();
	
}


template<int N>
void SimHandler<N>::writeDefaultParam(int id){
	switch(id){
		case 0:
			tools.simPort.writeEcho(false);
			break;
		case 1:
			tools.simPort.writeNumberFormat(false);
			break;
		case 2:
			tools.simPort.writeCallReady(false);
			break;
		case 3:
			tools.simPort.writeReportAsError(true);
			break;
		case 4:
			tools.simPort.writeIPR(115200);
			break;
		
		default : Serial.println("MISSID ID");
	}
}


template<int N>
bool SimHandler<N>::tryToSetDefaultParam(int id){
	bool result = true;
	
	writeDefaultParam(id);
	if(readAndGetCode(reader, tools.parser) == ANWSER_CODES::UNDEFINED){
		writeDefaultParam(id);
		result = tools.readAndExpectSuccess();
	}
	
	
	buffer.clear();
	return result;
}


template<int N>
void SimHandler<N>::handleTCPMessage(){
	auto tmp = tcpHandler.readMessage(buffer);
	TCPIncomingHandler handler;
	while(tmp.readResponce()){
		handler.handleMessage(buffer, refParams, tools.simPort, tools.parser);
	}
	
	tools.state.tcp.hasMessage = false;
}


template<int N>
bool SimHandler<N>::handleLongMessages(){
	if(tools.state.longCmd.cmdHandler){
		if(tools.state.longCmd.isAnwserReady){
			if(tools.state.longCmd.cmdHandler->handle()){
				tools.state.setLongCmd();	//longCmd to initial tools.state
				reader.handleSwitch();
				return false;
			}else{
				tools.state.longCmd.isAnwserReady = false;
				return true;
			}
		}else{
			if(wrapper.lazyRead()){
				tools.state.longCmd.isAnwserReady = true;
				return handleLongMessages();
			}
		}
		
		return true;
	}
	
	return false;
}