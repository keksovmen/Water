#include "PostDataHandler.h"
#include "Util.h"


PostDataHandler::PostDataHandler(	SimTools& tools,
									FixedBufferBase& buffer
									) :
	DataHandler(tools, buffer){
		
}


bool PostDataHandler::send(){
	this->refTools.writeEndOfCommand();
	
	if(!this->refTools.readAndExpectSuccess()){
		return false;
	}
		
	this->refTools.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	bool result = this->refTools.readAndExpectSuccess(LONG_WAIT);
	if(result){
		DataHandler::send();
	}
	
	return result;
}
