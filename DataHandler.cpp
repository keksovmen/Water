#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"




DataHandler::DataHandler(	SimResultParser& parser, 
								SimCommandPort& simPort,
								FixedBufferBase& buffer,
								SimState& state
								) :
	ResponceReader(parser, simPort, buffer),
	refState(state)
{
	
}




bool DataHandler::send(){
	refState.setLongCmd(this);
	return false;
}



bool DataHandler::handle(){
	if(this->refParser.isHttpActionPresents()){
		this->responceLength = this->refParser.fetchHttpResponceLength();
		refState.http.responseLength = this->responceLength;
		refState.http.responseCode = this->refParser.fetchHTTPStatus();
		refState.http.isAnwserReady = true;
		return true;
	}
	
	return false;
}



void DataHandler::write(const char* str){
	this->refPort.write(str);
}


void DataHandler::write(char c){
	this->refPort.write(c);
}


void DataHandler::write(int i){
	this->refPort.write(i);
}



void DataHandler::write(long l){
	this->refPort.write(l);
}


void DataHandler::write(double d, int amountAfterDot){
	this->refPort.write(d, amountAfterDot);
}



bool DataHandler::isSended(){
	return refState.http.isAnwserReady;
}



bool DataHandler::isSendedSuccesfully(){
	return refState.http.responseCode == 
		static_cast<int>(HTTP_STATUS_CODES::HTTP_STATUS_SUCCESS);
}



void DataHandler::finish(){
	this->refPort.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	readAndExpectSuccess(this->refPort, this->refParser);
	
	this->refBuffer.clear();
	refState.clearHTTP();
	
}



int DataHandler::getMinMessageLength(){
	return 22;
}



void DataHandler::removeGarbage(){
	this->refParser.removeReadGarbage(READ_TYPE_HTTP);
}



bool DataHandler::isMessageFull(){
	return this->refParser.isReadMessageFull(READ_TYPE_HTTP);
}



void DataHandler::askForData(int index, int amount){
	this->refPort.writeReadHTTP(index, amount);
}

