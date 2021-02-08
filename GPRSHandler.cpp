#include "GPRSHandler.h"
// #include "Enums.h"
// #include "Util.h"



GPRSHandler::GPRSHandler(SimTools& tools) : 
	refTools(tools)
{
	
}



bool GPRSHandler::handle(){
	if(!refTools.isSimpleMessageReady()){
		return false;
	}
	
	ANWSER_CODES code = static_cast<ANWSER_CODES>(
						refTools.fetchResultCode());
	switch(code){
		case OK:
		if(lastCommandOpen){
			refTools.state.health.GPRS_Connection = GPRS_CONNECTED;
		}else{
			refTools.state.health.GPRS_Connection = GPRS_CLOSED;
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
	refTools.state.health.GPRS_Connection = status;
	
	return status;
}


/**
	
	Max response time when open bearer is 85 sec
	
	@param apn must include double qouts on both sides
	@return true if bearer is opening or already open
*/



bool GPRSHandler::connect(const char* apn){
	refTools.writeSAPBR(SET_PARAM_BEARER,
						"Contype", "GPRS");
	
	if(!refTools.readAndExpectSuccess()){
		return false;
	}
	
	refTools.writeSAPBR(SET_PARAM_BEARER,
						"APN", apn);
	
	if(!refTools.readAndExpectSuccess()){
		return false;
	}
	
	refTools.writeSAPBR(OPEN_BEARER);
	refTools.state.setLongCmd(this);
	lastCommandOpen = true;
	
	return true;
}


/**
	Max response time when close bearer is 65 sec
	
	@return true is bearer is closing or already closed
*/


void GPRSHandler::close(){
	refTools.writeSAPBR(CLOSE_BEARER);
	refTools.state.setLongCmd(this);
	lastCommandOpen = false;
}


/**
	@return -1 if there is error of some sort
*/


int GPRSHandler::retriveStatus(){
	refTools.writeSAPBR(SAPBR_COMMANDS::QUERY_BEARER);
	
	if(!refTools.readAndExpectSuccess()){
		return -1;
	}
	
	return refTools.fetchGPRSStatus();
}