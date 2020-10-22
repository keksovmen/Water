#include "ParameterHandler.h"


int ParameterHandler::getLength(){
	return temp.getLength() + press.getLength() +
			clock.getLength() + 2;	//2: 2 & character
}

void ParameterHandler::handleWritingValue(BaseWriter& writer){
	//don't forget to add & between params
	temp.handleWritingValue(writer);
	writer.write('&');
	
	press.handleWritingValue(writer);
	writer.write('&');
	
	clock.handleWritingValue(writer);
}