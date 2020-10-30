#include "CgattHandler.h"
#include "Util.h"
#include "Enums.h"



template<int N>
CgattHandler<N>::CgattHandler(
				SimCommandPort& port, 
				SimResultParser<N>& parser,
				SimState& state
				):
	refPort(port), refParser(parser),
	refState(state)
{
	
}




template<int N>
bool CgattHandler<N>::handle(){
	if(!refParser.isSimpleMessageReady()){
		return false;
	}
	
	if(refParser.fetchResultCode() == OK){
		refState.health.CGATT_Connection = true;
	}
	
	return true;
}


template<int N>
bool CgattHandler<N>::connectToCGATT(){
	if(askCGATTStatus()){
		return true;
	}
	
	refPort.writeCGATT(CGATT_COMMANDS_ON);
	refState.setLongCmd(this);
	
	return false;
	// return readAndExpectSuccess(refPort, refParser, false, 7000);
}


template<int N>
bool CgattHandler<N>::askCGATTStatus(){
	refPort.writeCGATT(CGATT_COMMANDS_STATUS);
	if(!readAndExpectSuccess(refPort, refParser)){
		return false;
	}
	
	bool result = refParser.isAttachedToGPRSServices();
	refState.health.CGATT_Connection = result;
	
	return result;
}