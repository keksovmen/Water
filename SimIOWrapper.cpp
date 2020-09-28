#include <Arduino.h>
#include "SimIOWrapper.h"

//in millis
const static unsigned int MIN_DELAY = 25;

template<int N>
SimIOWrapper<N>::SimIOWrapper(SoftwareSerial& refSerial) : 
	refPort(refSerial){
		
}

template<int N>
bool SimIOWrapper<N>::writeCommand(const String& cmd){
	Serial.println(cmd);
	unsigned int written = refPort.println(cmd);
	buffer.clear();
	
	//- 2 because will be added \r\n
	return (written - 2) == cmd.length();
}

template<int N>
bool SimIOWrapper<N>::readToBuffer(){
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