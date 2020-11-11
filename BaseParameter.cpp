#include "BaseParameter.h"
#include "Util.h"
#include <Arduino.h>


static const int DIGITS_AFTER_DOT = 2;


int PrimitivIntParameter::getLength(){
	return findLongLength(value);
}


void PrimitivIntParameter::handleWritingValue(BaseWriter& writer){
	writer.write(value);
}


void PrimitivIntParameter::parse(const char* str){
	value = atol(str);
}


int PrimitivFloatParameter::getLength(){
	return findDoubleLength(value, DIGITS_AFTER_DOT);
}


void PrimitivFloatParameter::handleWritingValue(BaseWriter& writer){
	writer.write(value, DIGITS_AFTER_DOT);
}


void PrimitivFloatParameter::parse(const char* str){
	value = atof(str);
}
