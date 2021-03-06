#include "GetDataHandler.h"
#include "Util.h"



GetDataHandler::GetDataHandler(	SimTools& tools,
								FixedBufferBase& buffer
								) :
	DataHandler(tools, buffer){
		
}


bool GetDataHandler::send(){
	this->refTools.write('"');
	this->refTools.writeEndOfCommand();
			
	if(!this->refTools.readAndExpectSuccess()){
		return false;
	}
		
	this->refTools.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	bool result = this->refTools.readAndExpectSuccess(LONG_WAIT);
	if(result){
		DataHandler::send();
	}
	
	return result;
}