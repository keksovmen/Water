#include "ReaderWithoutGarbage.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"
#include <Arduino.h>



ReaderWithoutGarbage::ReaderWithoutGarbage(
				FixedBufferBase& buffer,
				BaseReader& reader,
				SimState& state
				) :
	refBuffer(buffer), refReader(reader),
	refState(state)
{

}


bool ReaderWithoutGarbage::read(){
	if(!refReader.read()){
		return false;
	}
	
	handleUnexpected(refBuffer);
	
	return true;
}


bool ReaderWithoutGarbage::readTimeout(unsigned long maxDelay){
	if(!refReader.readTimeout(maxDelay)){
		return false;
	}
	
	handleUnexpected(refBuffer);
	
	return true;
}


bool ReaderWithoutGarbage::lazyRead(){
	if(!refReader.lazyRead()){
		return false;
	}
	
	handleUnexpected(refBuffer);
	
	return true;
}


void ReaderWithoutGarbage::handleUnexpected(FixedBufferBase& refBuffer){
	
	if(refBuffer.remove(RING_STRING)){
		while(refBuffer.remove(RING_STRING)){}
		//change unexpectedMessages
		handleIncomingCall();
	}
	
	if(refBuffer.remove(NO_CARRIER_STRING)){
		
	}
	
	if(refBuffer.remove(UNDER_VOLTAGE_WARNING)){
		while(refBuffer.remove(UNDER_VOLTAGE_WARNING)){}
		
	}
	
	if(refBuffer.remove(OVER_VOLTAGE_WARNING)){
		while(refBuffer.remove(OVER_VOLTAGE_WARNING)){}
		
	}
	
	if(refBuffer.remove(SAPBR_DEACT)){
		refState.diedGPRS();
	}
	
	if(refBuffer.remove(TEXT_RDY)){
		refState.readyDetected();
	}
	
	UnexpectedHandler::handleUnexpected(refBuffer);
}



void ReaderWithoutGarbage::handleIncomingCall(){
	int lastIndex = refBuffer.indexOfEnd(NO_CARRIER_STRING);

	while(lastIndex == -1){
		readRemovingGarbage();
		lastIndex = refBuffer.indexOf(NO_CARRIER_STRING);
	}
	
	refBuffer.remove(NO_CARRIER_STRING);
}



bool ReaderWithoutGarbage::readRemovingGarbage(){
	if(!refReader.read()){
		return false;
	}
	
	while(refBuffer.remove(RING_STRING)){}
	
	while(refBuffer.remove(UNDER_VOLTAGE_WARNING)){}
	
	while(refBuffer.remove(OVER_VOLTAGE_WARNING)){}
	
	return true;
}

