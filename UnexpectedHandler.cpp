#include "UnexpectedHandler.h"
#include "Enums.h"
#include "Util.h"
#include <Arduino.h>


template<int N>
UnexpectedHandler<N>::UnexpectedHandler(SimIOWrapper<N>& wrapper, 
							SimResultParser<N>& parser,
							SimCommandWriter& writer) :
	wrapper(wrapper), parser(parser), writer(writer)
{

}



template<int N>
bool UnexpectedHandler<N>::read(){
	if(!wrapper.read()){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


template<int N>
bool UnexpectedHandler<N>::readTimeout(unsigned long maxDelay){
	if(!wrapper.readTimeout(maxDelay)){
		return false;
	}
	
	handleSwitch();
	
	return true;
}


template<int N>
void UnexpectedHandler<N>::handleSwitch(){
	auto& buffer = wrapper.getBuffer();
	
	// Serial.println("BUFFER 1:");
	// Serial.println(buffer.begin());
	// Serial.println("END");
	
	if(buffer.remove("\r\nRING\r\n")){
		while(buffer.remove("\r\nRING\r\n")){}
		//change unexpectedMessages
		handleIncomingCall();
	}
	
	if(buffer.remove("\r\nNO CARRIER\r\n")){
		
	}
	
	if(buffer.remove("\r\nUNDER-VOLTAGE WARNNING\r\n")){
		while(buffer.remove("\r\nUNDER-VOLTAGE WARNNING\r\n")){}
		
	}
	
	if(buffer.remove("\r\nOVER-VOLTAGE WARNNING\r\n")){
		while(buffer.remove("\r\nOVER-VOLTAGE WARNNING\r\n")){}
		
	}

	
	// Serial.println("BUFFER 2:");
	// Serial.println(buffer.begin());
	// Serial.println("END");
}


template<int N>
void UnexpectedHandler<N>::handleIncomingCall(){
	auto& buffer = wrapper.getBuffer();
	int lastIndex = buffer.indexOfEnd("\r\nNO CARRIER\r\n");
	// int index = buffer.indexOfEnd("\r\nOK\r\n");
	
	// Serial.print(lastIndex);
	// Serial.print("\t");
	// Serial.println(index);
	
	// writer.writeDenyCall();
	// int count = 0;
	while(lastIndex == -1){
		// if(count == 100){
			// writer.writeDenyCall();
			// count = 0;
		// }
		readRemovingGarbage();
		lastIndex = buffer.indexOf("\r\nNO CARRIER\r\n");
		// count++;
	}
	
	if(!buffer.remove("\r\nNO CARRIER\r\n")){
		Serial.println("FALSE REMOVE");
	}
	
	
	// lastIndex = buffer.indexOfEnd("\r\nOK\r\n");
	
	// while(lastIndex == index){
		// readRemovingGarbage();
		// lastIndex = buffer.indexOfEnd("\r\nOK\r\n");
		
		// Serial.print(index);
		// Serial.print("\t");
		// Serial.println(lastIndex);
		
		// Serial.println("BUFFER 4:");
		// Serial.println(buffer.begin());
		// Serial.println("END");
	// }
	
	
	// Serial.println("BUFFER 3:");
	// Serial.println(buffer.begin());
	// Serial.println("END");
	
	// buffer.remove(lastIndex, 6);
	// buffer.remove(index, 6);
}


template<int N>
bool UnexpectedHandler<N>::readRemovingGarbage(){
	auto& buffer = wrapper.getBuffer();
	
	if(!wrapper.read()){
		return false;
	}
	
	if(buffer.remove("\r\nRING\r\n")){
		while(buffer.remove("\r\nRING\r\n")){}
		
	}
	
	// if(buffer.remove("\r\nNO CARRIER\r\n")){
		
	// }
	
	if(buffer.remove("\r\nUNDER-VOLTAGE WARNNING\r\n")){
		while(buffer.remove("\r\nUNDER-VOLTAGE WARNNING\r\n")){}
		
	}
	
	if(buffer.remove("\r\nOVER-VOLTAGE WARNNING\r\n")){
		while(buffer.remove("\r\nOVER-VOLTAGE WARNNING\r\n")){}
		
	}
	
	return true;
}

