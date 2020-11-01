#include "GetDataHandler.h"
#include "Util.h"



GetDataHandler::GetDataHandler(	SimResultParser& parser, 
								SimCommandPort& simPort,
								FixedBufferBase& buffer,
								SimState& state
								) :
	DataHandler(parser, simPort, buffer, state){
		
}


bool GetDataHandler::send(){
	this->refPort.write('"');
	this->refPort.writeEndOfCommand();
			
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	bool result = readAndExpectSuccess(this->refPort, this->refParser);
	if(result){
		DataHandler::send();
	}
	
	return result;
}