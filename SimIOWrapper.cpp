#include <Arduino.h>
#include "SimIOWrapper.h"

//in millis
const static unsigned int MIN_DELAY = 25;

SimIOWrapper::SimIOWrapper(SoftwareSerial& refSerial) : 
	refPort(refSerial){
		
}

bool SimIOWrapper::writeCommand(const String& cmd){
	unsigned int written = refPort.println(cmd);
	buffer.clear();
	return written == cmd.length();
}

bool SimIOWrapper::readToBuffer(){
	if(refPort.available() == 0){
		delay(MIN_DELAY);
	}
	
	if(refPort.available() == 0){
		return false;
	}
	
	while(refPort.available()){
		if(buffer.isFull()){
			Serial.println("Buffer is full");
			return false;
		}
		char c = refPort.read();
		Serial.print(c);
		buffer += c; 
	}
	
	return true;
}