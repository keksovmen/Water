#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"



template <int N>
DataHandler<N>::DataHandler(	SimResultParser<N>& parser, 
								SimCommandPort& simPort,
								FixedBuffer<N>& buffer
								) :
	ResponceReader<N>(parser, simPort, buffer)
{
	
}

template<int N>
void DataHandler<N>::write(const char* str){
	this->refPort.write(str);
}

template<int N>
void DataHandler<N>::write(char c){
	this->refPort.write(c);
}

template<int N>
void DataHandler<N>::write(int i){
	this->refPort.write(i);
}


template<int N>
void DataHandler<N>::write(long l){
	this->refPort.write(l);
}

template<int N>
void DataHandler<N>::write(double d, int amountAfterDot){
	this->refPort.write(d, amountAfterDot);
}


template<int N>
bool DataHandler<N>::isSended(){
	if(this->refPort.read()){
		if(this->refParser.isHttpActionPresents()){
			this->responceLength = this->refParser.fetchHttpResponceLength();
			return true;
		}
	}
	
	return false;
}


template<int N>
bool DataHandler<N>::isSendedSuccesfully(){
	return this->refParser.fetchHTTPStatus() == 
		static_cast<int>(HTTP_STATUS_CODES::HTTP_STATUS_SUCCESS);
}


template<int N>
void DataHandler<N>::finish(){
	this->refPort.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	readAndExpectSuccess(this->refPort, this->refParser);
	this->refBuffer.clear();
}


template<int N>
int DataHandler<N>::getMinMessageLength(){
	return 22;
}


template<int N>
void DataHandler<N>::removeGarbage(){
	this->refParser.removeReadGarbage(READ_TYPE_HTTP);
}


template<int N>
bool DataHandler<N>::isMessageFull(){
	return this->refParser.isReadMessageFull(READ_TYPE_HTTP);
}


template<int N>
void DataHandler<N>::askForData(int index, int amount){
	this->refPort.writeReadHTTP(index, amount);
}

