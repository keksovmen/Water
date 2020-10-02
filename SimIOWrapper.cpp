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
void SimIOWrapper<N>::writeCommand(const char* cmd, bool clearBuffer){
	writeString(cmd);
	writeEndOfCommand(clearBuffer);
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
void SimIOWrapper<N>::writeEndOfCommand(bool clearBuffer){
	// #ifdef ABS
		Serial.print("\r\n");
	// #endif
	refPort.print("\r\n");
	
	if(clearBuffer){
		buffer.clear();
	}
}



template<int N>
bool SimIOWrapper<N>::readToBuffer(){
	if(tryReadToBuffer()){
		while(tryReadToBuffer()){
			
		}
			
		return true;
	}
	
	return false;
}

template<int N>
bool SimIOWrapper<N>::readToBufferTimeout(int millis){
	while(millis > 0){
		if(tryReadToBuffer()){
			while(tryReadToBuffer()){
				
			}
			
			return true;
		}
		
		millis -= MIN_DELAY;
	}
	
	return false;
}


template<int N>
bool SimIOWrapper<N>::tryReadToBuffer(){
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

