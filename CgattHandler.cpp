#include "CgattHandler.h"
#include "Util.h"
#include "Enums.h"




CgattHandler::CgattHandler(
				SimCommandPort& port, 
				SimResultParser& parser,
				SimState& state
				):
	refPort(port), refParser(parser),
	refState(state)
{
	
}





bool CgattHandler::handle(){
	if(!refParser.isSimpleMessageReady()){
		return false;
	}
	
	if(refParser.fetchResultCode() == OK){
		refState.health.CGATT_Connection = true;
	}
	
	return true;
}



bool CgattHandler::connectToCGATT(){
	if(askCGATTStatus()){
		return true;
	}
	
	refPort.writeCGATT(CGATT_COMMANDS_ON);
	refState.setLongCmd(this);
	
	return false;
}



bool CgattHandler::askCGATTStatus(){
	refPort.writeCGATT(CGATT_COMMANDS_STATUS);
	if(!readAndExpectSuccess(refPort, refParser)){
		return false;
	}
	
	bool result = refParser.isAttachedToGPRSServices();
	refState.health.CGATT_Connection = result;
	
	return result;
}