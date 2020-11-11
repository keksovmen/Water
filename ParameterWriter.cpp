#include "ParameterWriter.h"


bool ParameterWriter::add(BaseParameter* param){
	if (index >= SIZE_OF_PARAMETER_WRITER){
		return false;
	}
	
	params[index] = param;
	index++;
	
	return true;
}


int ParameterWriter::getLength() {
	int length = 0;
	for(int i = 0; i < index; i++){
		length += params[i]->getLength();
	}
	
	length += index - 1;
	
	return length;
}

		
void ParameterWriter::handleWritingValue(BaseWriter& writer){
	for(int i = 0; i < index; i++){
		params[i]->handleWritingValue(writer);
		if(i < index - 1){
			writer.write('&');
		}
	}
}


void ParameterWriter::parse(const char* str){
	//don't to anything
}