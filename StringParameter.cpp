#include "StringParameter.h"


int StringParameter::getLength(){
	return value.length();
}


void StringParameter::handleWritingValue(BaseWriter& writer){
	writer.write(value.c_str());
}


void StringParameter::setValue(const char* str){
	value = String(str);
}


const char* StringParameter::getValue(){
	return value.c_str();
}