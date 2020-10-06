#include "ParameterHandler.h"


int ParameterHandler::getLength(){
	return temp.getLength() + press.getLength() + 3;	//3 for two = and one &
}

void ParameterHandler::handleWritingValue(BaseWriter& writer){
	//first write id then = then value then & repeat
	writer.writeInt(temp.getId());
	writer.writeChar('=');
	temp.handleWritingValue(writer);
	
	writer.writeChar('&');
	
	writer.writeInt(press.getId());
	writer.writeChar('=');
	press.handleWritingValue(writer);
}