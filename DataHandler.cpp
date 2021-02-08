#include "DataHandler.h"
#include "Enums.h"
#include "Util.h"
#include "StringData.h"




DataHandler::DataHandler(	SimTools& tools,
							FixedBufferBase& buffer
							) :
	ResponceReader(tools, buffer)
	// this->refTools.state(state)
{
	
}




bool DataHandler::send(){
	this->refTools.state.setLongCmd(this);
	return false;
}



bool DataHandler::handle(){
	if(this->refTools.isHttpActionPresents()){
		this->responceLength = this->refTools.fetchHttpResponceLength();
		this->refTools.state.http.responseLength = this->responceLength;
		this->refTools.state.http.responseCode = this->refTools.fetchHTTPStatus();
		this->refTools.state.http.isAnwserReady = true;
		return true;
	}
	
	return false;
}



void DataHandler::write(const char* str){
	this->refTools.write(str);
}


void DataHandler::write(char c){
	this->refTools.write(c);
}


void DataHandler::write(int i){
	this->refTools.write(i);
}



void DataHandler::write(long l){
	this->refTools.write(l);
}


void DataHandler::write(double d, int amountAfterDot){
	this->refTools.write(d, amountAfterDot);
}



bool DataHandler::isSended(){
	return this->refTools.state.http.isAnwserReady;
}



bool DataHandler::isSendedSuccesfully(){
	return this->refTools.state.http.responseCode == 
		static_cast<int>(HTTP_STATUS_CODES::HTTP_STATUS_SUCCESS);
}



void DataHandler::finish(){
	this->refTools.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	this->refTools.readAndExpectSuccess();
	
	this->refBuffer.clear();
	this->refTools.state.clearHTTP();
	
}


/**
	@return \r\nHTTPREAD:<length>\r\n\r\nOK\r\n
		where, <length> is 1 digit
*/


int DataHandler::getMinMessageLength(){
	return strlen(HTTP_HTTPREAD_ANWSER) +
			strlen(TEXT_SUCCESS) + 3;
}



void DataHandler::removeGarbage(){
	this->refTools.removeReadGarbage(READ_TYPE_HTTP);
}



bool DataHandler::isMessageFull(){
	return this->refTools.isReadMessageFull(READ_TYPE_HTTP);
}



void DataHandler::askForData(int index, int amount){
	this->refTools.writeReadHTTP(index, amount);
}

