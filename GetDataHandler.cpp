#include "GetDataHandler.h"
#include "Util.h"



GetDataHandler::GetDataHandler(	SimTools& tools,
								FixedBufferBase& buffer
								) :
	DataHandler(tools, buffer){
		
}


bool GetDataHandler::send(){
	this->refTools.simPort.write('"');
	this->refTools.simPort.writeEndOfCommand();
			
	if(!this->refTools.readAndExpectSuccess()){
		return false;
	}
		
	this->refTools.simPort.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	bool result = this->refTools.readAndExpectSuccess(LONG_WAIT);
	if(result){
		DataHandler::send();
	}
	
	return result;
}