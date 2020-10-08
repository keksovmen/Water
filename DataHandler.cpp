#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"


template <int N>
DataHandler<N>::DataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer) :
	refWrapper (wrapper), refParser(parser), refWriter(writer){
		
}

template<int N>
void DataHandler<N>::write(const char* str){
	refWrapper.write(str);
}

template<int N>
void DataHandler<N>::write(char c){
	refWrapper.write(c);
}

template<int N>
void DataHandler<N>::write(int i){
	refWrapper.write(i);
}


template<int N>
void DataHandler<N>::write(long l){
	refWrapper.write(l);
}

template<int N>
void DataHandler<N>::write(double d, int amountAfterDot){
	refWrapper.write(d, amountAfterDot);
}


template<int N>
bool DataHandler<N>::isSended(){
	if(refWrapper.readToBuffer()){
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
	refWriter.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	readAndExpectSuccess(refWrapper, refParser);
}


/**
	ATTENTION!
		YOU MUST EMPTY THE BUFFER!
	
	Basicly call in while loop, it will fill buffer
	with 64 - some treshhold data length
	
	\r\n+HTTPREAD: <xx>\r\n - 15 garbage symbols + 1 for each <x>
	dataOK\r\n - 3 garbage symbols
	
	So should ask for 64 - 20 = 44 bytes of data,
	because of UART 64 bytes buffer
	//TODO: there could be undervoltage and other shit messages
	//handle it somehow
	
*/

template<int N>
bool DataHandler<N>::readResponce(){
	if(readIndex >= responceLength){
		return false;
	}
	
	const int MIN_LENGTH = 19;
	//if less than 19 symbols left return buffer full
	if(refWrapper.getBuffer().remains() < MIN_LENGTH){
		//TODO: made read to buffer return actual amount readed
		//so you can fetch how much did you read
		Serial.println("BUFFER OVERFLOW");
		return true;
	}

	unsigned int readAmount = refWrapper.getBuffer().remains() - MIN_LENGTH;
	//10 symbol fix
	if(readAmount > responceLength){
		readAmount = responceLength;
		
	}else if(readAmount == 10){
		readAmount = 9;
		
	}else{
		readAmount -= findLongLength(readAmount);
	}
	
	refWriter.writeReadHTTP(readIndex, readAmount);
	readIndex += readAmount;
	
	while(1){
		refWrapper.readToBuffer();
		if(refParser.isReadHttpMessageFull()){
			break;
		}
	}
	
	//remove all garbage
	refParser.removeReadHttpGarbage();
	
	return true;
}

