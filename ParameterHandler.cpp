#include "ParameterHandler.h"
#include <Arduino.h>


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


PARAMETER_PARSER_RESPONCES ParameterHandler::parse(FixedBufferBase& buffer){
	int begining = findBeginingOfParam(buffer);
	int ending = findEndOfParam(buffer);
	if(begining == -1){
		return PARAMETER_PARSER_NO_MORE_ENTRIES;
	}
	
	if(ending == -1){
		return PARAMETER_PARSER_NEED_MORE_DATA;
	}
	
	if(hasPing(buffer, begining)){
		removeParamEntry(buffer, begining, ending);
		return PARAMETER_PARSER_PING_FOUND;
	}
	
	handleSwitchParsing(buffer, begining);
	removeParamEntry(buffer, begining, ending);
	
	return PARAMETER_PARSER_ENTRY_PARSED;
}


int ParameterHandler::findBeginingOfParam(FixedBufferBase& buffer){
	return buffer.indexOf(ENTRY_BEGINING);
}


int ParameterHandler::findEndOfParam(FixedBufferBase& buffer){
	return buffer.indexOf(ENTRY_ENDING);
}


bool ParameterHandler::hasPing(FixedBufferBase& buffer, int begining){
	return buffer.indexOfFrom(begining, "PING") != -1;
}


void ParameterHandler::removeParamEntry(FixedBufferBase& buffer, int begining, int end){
	int amount = (end - begining) + 1;
	buffer.remove(begining, amount);
}


int ParameterHandler::parseIdOfParam(FixedBufferBase& buffer, int begining){
	return atoi(&buffer[begining + 1]);
}


int ParameterHandler::findParamBegining(FixedBufferBase& buffer, int begining){
	return buffer.indexOfFrom(begining, "=") + 1;
}


void ParameterHandler::handleSwitchParsing(FixedBufferBase& buffer, int begining){
	int id = parseIdOfParam(buffer, begining);
	int paramBegining = findParamBegining(buffer, begining);
	const char* str = &buffer[paramBegining];
	
	switch(id){
		case 2:
			clock.parse(str);
			break;
		case 3:
			address.parse(str);
			break;
		case 4:
			userVolume.parse(str);
			break;
		case 7:
			apn.parse(str);
		case 8:
			tempUp.parse(str);
			break;
		case 9:
			tempDown.parse(str);
			break;
		case 10:
			plateID.parse(str);
			break;
			
		default: break;	
	}
}