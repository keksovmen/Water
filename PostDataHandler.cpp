#include "PostDataHandler.h"
#include "Util.h"


PostDataHandler::PostDataHandler(	SimResultParser& parser, 
									SimCommandPort& simPort,
									FixedBufferBase& buffer,
									SimState& state
									) :
	DataHandler(parser, simPort, buffer, state){
		
}


bool PostDataHandler::send(){
	this->refPort.writeEndOfCommand();
	
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	bool result = readAndExpectSuccess(this->refPort, this->refParser);
	if(result){
		DataHandler::send();
	}
	
	return result;
}
