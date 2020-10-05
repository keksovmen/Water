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
void SimIOWrapper<N>::writeString(const char* str){
	// #ifdef ABS
		Serial.print(str);
	// #endif
	refPort.print(str);
}


template<int N>
void SimIOWrapper<N>::writeChar(char c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
}


template<int N>
void SimIOWrapper<N>::writeInt(int i){
	// #ifdef ABS
		Serial.print(i);
	// #endif
	refPort.print(i);
}


template<int N>
void SimIOWrapper<N>::writeDouble(double d, int amountAfterDot){
	// #ifdef ABS
		Serial.print(d, amountAfterDot);
	// #endif
	refPort.print(d, amountAfterDot);
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
	//if there is no data wait
	if(refPort.available() == 0){
		delay(MIN_DELAY);
	}
	
	//if there is still no data return
	if(refPort.available() == 0){
		return false;
	}
	
	//read as much as possible
	while(refPort.available()){
		
		//WARNING error place
		//try to find optimal buffer size
		//to prevent from overfloving
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

