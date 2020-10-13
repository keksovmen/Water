#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"


//TODO: made explicitly buffer reader and writer and parser

template <int N>
DataHandler<N>::DataHandler(CommandWriter& wrapper, 
								SimResultParser<N>& parser, 
								SimCommandWriter& writer,
								BaseReader& reader,
								FixedBuffer<N>& buffer) :
	refWriteHandler (wrapper), refParser(parser), 
	refWriter(writer), refReader(reader), refBuffer(buffer){
		
}

template<int N>
void DataHandler<N>::write(const char* str){
	refWriteHandler.write(str);
}

template<int N>
void DataHandler<N>::write(char c){
	refWriteHandler.write(c);
}

template<int N>
void DataHandler<N>::write(int i){
	refWriteHandler.write(i);
}


template<int N>
void DataHandler<N>::write(long l){
	refWriteHandler.write(l);
}

template<int N>
void DataHandler<N>::write(double d, int amountAfterDot){
	refWriteHandler.write(d, amountAfterDot);
}


template<int N>
bool DataHandler<N>::isSended(){
	if(refReader.read()){
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
	readAndExpectSuccess(refReader, refParser);
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
	//10 symbol fix
	if(readAmount > 64){
		readAmount = 64;
		
	}else if(readAmount == 10){
		readAmount = 9;
		
	}else{
		readAmount -= findLongLength(readAmount);
	}
	
	refWriter.writeReadHTTP(readIndex, readAmount);
	readIndex += readAmount;
	
	while(1){
		refReader.read();
		// Serial.println("<---->");
		// Serial.println(refBuffer.begin());
		// Serial.println("<---->");
		if(refParser.isReadHttpMessageFull()){
			// Serial.println("BREAK");
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

