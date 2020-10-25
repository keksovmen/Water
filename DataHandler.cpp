#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"



template <int N>
DataHandler<N>::DataHandler(	SimResultParser<N>& parser, 
								SimCommandPort& simPort,
								FixedBuffer<N>& buffer
								) :
	refParser(parser), refPort(simPort), 
	refBuffer(buffer)
{
	
}

template<int N>
void DataHandler<N>::write(const char* str){
	refPort.write(str);
}

template<int N>
void DataHandler<N>::write(char c){
	refPort.write(c);
}

template<int N>
void DataHandler<N>::write(int i){
	refPort.write(i);
}


template<int N>
void DataHandler<N>::write(long l){
	refPort.write(l);
}

template<int N>
void DataHandler<N>::write(double d, int amountAfterDot){
	refPort.write(d, amountAfterDot);
}


template<int N>
bool DataHandler<N>::isSended(){
	if(refPort.read()){
		if(refParser.isHttpActionPresents()){
			responceLength = refParser.fetchHttpResponceLength();
			return true;
		}
	}
	
	return false;
}


template<int N>
bool DataHandler<N>::isSendedSuccesfully(){
	return refParser.fetchHTTPStatus() == 
		static_cast<int>(HTTP_STATUS_CODES::HTTP_STATUS_SUCCESS);
}


template<int N>
void DataHandler<N>::finish(){
	refPort.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	readAndExpectSuccess(refPort, refParser);
	refBuffer.clear();
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

template<int N>
bool DataHandler<N>::readResponce(){
	if(firstRead){
		refBuffer.clear();
		firstRead = false;
	}
	
	if(readIndex >= responceLength){
		return false;
	}
	
	const int MIN_LENGTH = 22;
	//if less than 22 symbols left return buffer full
	if(refBuffer.remains() < MIN_LENGTH){
		//TODO: made read to buffer return actual amount readed
		//so you can fetch how much did you read
		Serial.println("READ_RESPONCE");
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
	
	refPort.writeReadHTTP(readIndex, readAmount);
	readIndex += readAmount;
	
	while(1){
		refPort.read();
		if(refParser.isReadHttpMessageFull()){
			break;
		}else{
			if(refParser.checkError()){
				return false;
			}
		}
	}
	
	//remove all garbage
	refParser.removeReadHttpGarbage();
	
	return true;
}

