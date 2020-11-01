#include "GPRSHandler.h"
#include "Enums.h"
#include "Util.h"



GPRSHandler::GPRSHandler(
					SimCommandPort& simPort,
					SimResultParser& parser,
					SimState& state
					) : 
	refPort(simPort), refParser(parser),
	refState(state)
{
	
}



bool GPRSHandler::handle(){
	if(!refParser.isSimpleMessageReady()){
		return false;
	}
	
	ANWSER_CODES code = static_cast<ANWSER_CODES>(
						refParser.fetchResultCode());
	switch(code){
		case OK:
		if(lastCommandOpen){
			refState.health.GPRS_Connection = GPRS_CONNECTED;
		}else{
			refState.health.GPRS_Connection = GPRS_CLOSED;
		}
			break;
		default: break;
	}

	return true;
}



BEARER_STATUS GPRSHandler::isConnected(){
	int result = (retriveStatus());
	
	if(result == -1){
		return GPRS_UNDEFINIED;
	}
	
	BEARER_STATUS status = static_cast<BEARER_STATUS>(result);
	refState.health.GPRS_Connection = status;
	
	return status;
}


/**
	
	Max response time when open bearer is 85 sec
	
	@param apn must include double qouts on both sides
	@return true if bearer is opening or already open
*/



bool GPRSHandler::connect(const char* apn){
	refPort.writeSAPBR(SET_PARAM_BEARER,
						"Contype", "GPRS");
	
	if(!readAndExpectSuccess(refPort, refParser)){
		return false;
	}
	
	refPort.writeSAPBR(SET_PARAM_BEARER,
						"APN", apn);
	
	if(!readAndExpectSuccess(refPort, refParser)){
		return false;
	}
	
	refPort.writeSAPBR(OPEN_BEARER);
	refState.setLongCmd(this);
	lastCommandOpen = true;
	
	return true;
}


/**
	Max response time when close bearer is 65 sec
	
	@return true is bearer is closing or already closed
*/


void GPRSHandler::close(){
	refPort.writeSAPBR(CLOSE_BEARER);
	refState.setLongCmd(this);
	lastCommandOpen = false;
}


/**
	@return -1 if there is error of some sort
*/


int GPRSHandler::retriveStatus(){
	refPort.writeSAPBR(SAPBR_COMMANDS::QUERY_BEARER);
	
	if(!readAndExpectSuccess(refPort, refParser, true)){
		return -1;
	}
	
	return refParser.fetchGPRSStatus();
}