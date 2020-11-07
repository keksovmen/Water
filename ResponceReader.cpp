#include "ResponceReader.h"

#include <Arduino.h>
#include "Util.h"



ResponceReader::ResponceReader(		
					SimTools& tools,
					FixedBufferBase& buffer
					) :
	refTools(tools),
	refBuffer(buffer)
{
	
}



/**
	ATTENTION!
		YOU MUST EMPTY THE BUFFER!
	
	Basicly call in while loop, it will fill buffer
	with 64 - some treshhold data length
	
	\r\n+HTTPREAD: <xx>\r\n - 15 garbage symbols + 1 for each <x>
	data\r\nOK\r\n - 6 garbage symbols
	
	So should ask for 64 - 21 = 41 bytes of data,
	because of UART 64 bytes buffer
	//TODO: there could be undervoltage and other shit messages
	//handle it somehow
	
*/


bool ResponceReader::readResponce(){
	if(firstRead){
		refBuffer.clear();
		firstRead = false;
	}
	
	if(readIndex >= responceLength){
		return false;
	}
	
	const int MIN_LENGTH = getMinMessageLength();
	//if less than symbols left return buffer full
	if(refBuffer.remains() < MIN_LENGTH){
		//TODO: made read to buffer return actual amount readed
		//so you can fetch how much did you read
		Serial.println("READ_RESPONCE_BUFFER_LOW");
		return true;
	}

	unsigned int readAmount = refBuffer.remains() - MIN_LENGTH;
	//TODO: Made use UART define buffer size
	if(readAmount > 64){
		readAmount = 64;
		
	//10 symbol fix
	}else if(readAmount == 10){
		readAmount = 9;
		
	}else{
		readAmount -= findLongLength(readAmount);
	}
	
	askForData(readIndex, readAmount);
	readIndex += readAmount;
	
	while(1){
		if(!refTools.simPort.read()){
			return false;
		}
			
		if(isMessageFull()){
			break;
		}else{
			if(refTools.parser.checkError()){
				return false;
			}
		}
	}
	
	//remove all garbage
	removeGarbage();
	
	return true;
}