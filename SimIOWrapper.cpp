#include "SimIOWrapper.h"

// #ifdef ABS
	// #warning definied
	// #include <Arduino.h>
// #endif


//in millis
const static unsigned int MIN_DELAY = 25;

template<int N>
SimIOWrapper<N>::SimIOWrapper(SoftwareSerial& refSerial) : 
	refPort(refSerial){
		
}

template<int N>
void SimIOWrapper<N>::writeCommand(const char* cmd){
	writeString(cmd);
	writeEndOfCommand();
}

template<int N>
SimIOWrapper<N>& SimIOWrapper<N>::writeChar(char c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
	
	return *this;
}

template<int N>
SimIOWrapper<N>& SimIOWrapper<N>::writeInt(int c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
	
	return *this;
}

template<int N>
SimIOWrapper<N>& SimIOWrapper<N>::writeString(const char* c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
	
	return *this;
}

template<int N>
void SimIOWrapper<N>::writeEndOfCommand(){
	// #ifdef ABS
		Serial.print("\r\n");
	// #endif
	refPort.print("\r\n");
	buffer.clear();
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
			// #ifdef ABS
				Serial.println("Buffer is full");
			// #endif
			
			return true;
		}
		char c = refPort.read();
		
		// #ifdef ABS
			Serial.print(c);
		// #endif
		
		buffer += c; 
	}
	
	return true;
}

template<int N>
bool SimIOWrapper<N>::readToBufferTimeout(int millis){
	while(millis > 0){
		if(readToBuffer()){
			return true;
		}
		
		millis -= MIN_DELAY;
	}
	
	return false;
}

