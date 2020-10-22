#include "SimIOWrapper.h"
#include "StringData.h"

// #ifdef ABS
	// #warning definied
	// #include <Arduino.h>
// #endif




template<int N>
SimIOWrapper<N>::SimIOWrapper(Stream& refSerial) : 
	refPort(refSerial){
		
}


template<int N>
void SimIOWrapper<N>::write(const char* str){
	// #ifdef ABS
		Serial.print(str);
	// #endif
	int amount = refPort.print(str);
	if(amount != strlen(str)){
		Serial.println("Amount is missing");
	}
}


template<int N>
void SimIOWrapper<N>::write(char c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
}


template<int N>
void SimIOWrapper<N>::write(int i){
	// #ifdef ABS
		Serial.print(i);
	// #endif
	refPort.print(i);
}

template<int N>
void SimIOWrapper<N>::write(long l){
	// #ifdef ABS
		Serial.print(l);
	// #endif
	refPort.print(l);
}


template<int N>
void SimIOWrapper<N>::write(double d, int amountAfterDot){
	// #ifdef ABS
		Serial.print(d, amountAfterDot);
	// #endif
	refPort.print(d, amountAfterDot);
}


template<int N>
void SimIOWrapper<N>::writeEndOfCommand(bool clearBuffer){
	// #ifdef ABS
		Serial.print(END_LINE);
	// #endif
	//Just for test purposes to see the difference
	refPort.print('\r');
	
	if(clearBuffer){
		buffer.clear();
	}
}


template<int N>
bool SimIOWrapper<N>::read(){
	if(tryReadToBuffer()){
		while(tryReadToBuffer()){
			
		}
		
		return true;
	}
	
	return false;
}


template<int N>
bool SimIOWrapper<N>::readTimeout(unsigned long maxDelay){
	while(maxDelay > 0){
		if(tryReadToBuffer()){
			while(tryReadToBuffer()){
				
			}
			
			return true;
		}
		
		maxDelay -= WRAPPER_MIN_DELAY;
	}
	
	return false;
}


template<int N>
bool SimIOWrapper<N>::lazyRead(){
	if(refPort.available() == 0){
		return false;
	}
	
	//prevent from looping if buffer full
	if(buffer.remains() == 0){
		return false;
	}
	
	//read as much as possible
	while(refPort.available()){
		
		//WARNING error place
		//try to find optimal buffer size
		//to prevent from overfloving
		if(buffer.remains() == 0){
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
bool SimIOWrapper<N>::tryReadToBuffer(){
	//if there is no data wait
	if(!lazyRead()){
		delay(WRAPPER_MIN_DELAY);
	}else{
		return true;
	}
	
	return lazyRead();
}

