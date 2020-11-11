#include "ParameterHandler.h"


// int ParameterHandler::getLength(){
	// return temp.getLength() + press.getLength() +
			// clock.getLength() + 2;	//2: 2 & character
// }

// void ParameterHandler::handleWritingValue(BaseWriter& writer){
	// don't forget to add & between params
	// temp.handleWritingValue(writer);
	// writer.write('&');
	
	// press.handleWritingValue(writer);
	// writer.write('&');
	
	// clock.handleWritingValue(writer);
// }

bool ParameterHandler::hasId(int id){
	if(id < 0 || id > 9){
		return false;
	}
	
	return true;
}


void ParameterHandler::parse(int id, const char* str){
	switch(id){
		case 2:
			clock.parse(str);
			break;
		case 3:
			address.parse(str);
			break;
		case 7:
			apn.parse(str);
		case 8:
			tempUp.parse(str);
			break;
		case 9:
			tempDown.parse(str);
			break;
			
		default: break;	
	}
}