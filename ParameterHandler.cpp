#include "ParameterHandler.h"


int ParameterHandler::getLength(){
	return temp.getLength() + press.getLength() +
			clock.getLength() + 5;	//5: 3 = and 2 &
}

void ParameterHandler::handleWritingValue(BaseWriter& writer){
	//first write id then = then value then & repeat
	writer.write(temp.getId());
	writer.write('=');
	temp.handleWritingValue(writer);
	
	writer.write('&');
	
	writer.write(press.getId());
	writer.write('=');
	press.handleWritingValue(writer);
	
	writer.write('&');
	
	writer.write(clock.getId());
	writer.write('=');
	clock.handleWritingValue(writer);
}