#include "IPAddressParam.h"
#include "Util.h"


int IPAddressParam::getLength(){
	return findLongLength(this[0]) +
	findLongLength(this[1]) +
	findLongLength(this[2]) +
	findLongLength(this[3]);
}


void IPAddressParam::handleWritingValue(BaseWriter& writer){
	writer.write((int) this[0]);
	writer.write('.');
	writer.write((int) this[1]);
	writer.write('.');
	writer.write((int) this[2]);
	writer.write('.');
	writer.write((int) this[3]);
}


void IPAddressParam::parse(const char* str){
	fromString(str);
}