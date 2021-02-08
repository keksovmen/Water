#include "SimCommandPort.h"




SimCommandPort::SimCommandPort(CommandWriter& writer, BaseReader& reader) :
		SimCommandWriter(writer), refReader(reader)
{
	
}


bool SimCommandPort::read(){
	return refReader.read();
}


bool SimCommandPort::readTimeout(unsigned long maxDelay){
	return refReader.readTimeout(maxDelay);
}


bool SimCommandPort::lazyRead(){
	return refReader.lazyRead();
}