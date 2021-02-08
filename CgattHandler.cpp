#include "CgattHandler.h"



CgattHandler::CgattHandler(SimTools& tools):
	refTools(tools)
{
	
}


bool CgattHandler::handle(){
	//TODO: made single method for only text check, fuck digit
	if(!refTools.isSimpleMessageReady()){
		return false;
	}
	
	if(refTools.fetchResultCode() == OK){
		refTools.state.health.CGATT_Connection = true;
	}
	
	return true;
}



bool CgattHandler::connectToCGATT(){
	if(askCGATTStatus()){
		return true;
	}
	
	refTools.writeCGATT(CGATT_COMMANDS_ON);
	refTools.state.setLongCmd(this);
	
	return false;
}



bool CgattHandler::askCGATTStatus(){
	refTools.writeCGATT(CGATT_COMMANDS_STATUS);
	if(!refTools.readAndExpectSuccess()){
		return false;
	}
	
	bool result = refTools.isAttachedToGPRSServices();
	refTools.state.health.CGATT_Connection = result;
	
	return result;
}