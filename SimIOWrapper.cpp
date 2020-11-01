#include "SimIOWrapper.h"
#include "StringData.h"

// #ifdef ABS
	// #warning definied
	#include <Arduino.h>
// #endif





SimIOWrapper::SimIOWrapper(
			Stream& refSerial, 
			FixedBufferBase& buffer
			) : 
	refPort(refSerial), refBuffer(buffer)
{
	
}



void SimIOWrapper::write(const char* str){
	// #ifdef ABS
		Serial.print(str);
	// #endif
	// refPort.print(str);
	int amount = refPort.print(str);
	if(amount != strlen(str)){
		Serial.println("Amount is missing");
	}
}



void SimIOWrapper::write(char c){
	// #ifdef ABS
		Serial.print(c);
	// #endif
	refPort.print(c);
}



void SimIOWrapper::write(int i){
	// #ifdef ABS
		Serial.print(i);
	// #endif
	refPort.print(i);
}


void SimIOWrapper::write(long l){
	// #ifdef ABS
		Serial.print(l);
	// #endif
	refPort.print(l);
}



void SimIOWrapper::write(double d, int amountAfterDot){
	// #ifdef ABS
		Serial.print(d, amountAfterDot);
	// #endif
	refPort.print(d, amountAfterDot);
}



void SimIOWrapper::writeEndOfCommand(bool clearBuffer){
	// #ifdef ABS
		Serial.print(END_LINE);
	// #endif
	//Just for test purposes to see the difference
	refPort.print('\r');
	
	if(clearBuffer){
		refBuffer.clear();
	}
}



bool SimIOWrapper::read(){
	if(tryReadToBuffer()){
		while(tryReadToBuffer()){
			
		}
		
		return true;
	}
	
	return false;
}



bool SimIOWrapper::readTimeout(unsigned long maxDelay){
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



bool SimIOWrapper::lazyRead(){
	if(refPort.available() == 0){
		return false;
	}
	
	//prevent from looping if refBuffer full
	if(refBuffer.remains() == 0){
		return false;
	}
	
	//read as much as possible
	while(refPort.available()){
		
		//WARNING error place
		//try to find optimal refBuffer size
		//to prevent from overfloving
		if(refBuffer.remains() == 0){
			return true;
		}
		
		char c = refPort.read();
		
		// #ifdef ABS
			Serial.print(c);
		// #endif
		
		refBuffer += c; 
	}
	
	return true;
}


bool SimIOWrapper::tryReadToBuffer(){
	//if there is no data wait
	if(!lazyRead()){
		delay(WRAPPER_MIN_DELAY);
	}else{
		return true;
	}
	
	return lazyRead();
}

