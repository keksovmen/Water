#include "ResultParserState.h"

#include <Arduino.h>
#include "StringData.h"




bool ResultParserStateBase::isSimpleMessageReady(){
	if(checkError())
		return true;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;
	

	
	return false;
}



bool ResultParserStateBase::isComplexMessageReady(){
	if(this->refBuffer.getLength() <= 3){
		return false;
	}
	
	if(checkError())
		return true;
	
	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_COMPLEX_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_COMPLEX_ERROR) != -1)
		return true;

	
	return false;
}


/**
	Override for different states
*/

bool ResultParserStateBase::removeResultCode(){
	return false;
}


/**
	NOT CALL FROM SUPER CLASS DUE TO LACK OF KNOLEGE
	OF WHAT TYPE OF MESSAGE DEAL WITH
	
	Subclass must provide correct SimpleMEssageREady
	you can't sa for sure is read message full or not
*/


bool ResultParserStateBase::isReadMessageFull(READ_TYPE type){
	const char* str = routeReadType(type);
	int lastIndexForRead = findLastIndexForRead(str);
	if(lastIndexForRead == -1){
		return false;
	}
	
	if(this->refBuffer.getLength() < lastIndexForRead + getAmountToDeleteAfterRead()){
		return false;
	}
	

	return isReadEnded(lastIndexForRead + 1);
}
 
 
 
bool ResultParserStateBase::checkError(){
	int index = this->refBuffer.indexOfEnd(TEXT_CME_ERROR);
	if(this->refBuffer.indexOfFrom(index, END_LINE) != -1){
		index += strlen(TEXT_CME_ERROR);
		lastErrorCode = atoi(&this->refBuffer[index]);
		
		return true;
	}
	
	lastErrorCode = -1;
	return false;
}



int ResultParserStateBase::fetchResultCode(){
	if(checkError())
		return ANWSER_CODES::ERROR;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;
	
	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;


	return ANWSER_CODES::UNDEFINED;	
}


/**
	NOT CALL FROM SUPER CLASS DUE TO LACK OF KNOLEGE
	OF WHAT TYPE OF MESSAGE DEAL WITH
*/


void ResultParserStateBase::removeReadGarbage(READ_TYPE type){
	const char* str = routeReadType(type);
	int index = this->refBuffer.indexOf(str);
	int endIndex = this->refBuffer.indexOfFrom(
			index + strlen(str), END_LINE);
	//set on first character of real data
	endIndex += strlen(END_LINE);
	
	//remove from right to left cause everything will fuck up 
	//the other way around!
	this->refBuffer.remove(findLastIndexForRead(str)+ 1, 
					getAmountToDeleteAfterRead());
					
	this->refBuffer.remove(index, endIndex - index);
	
}


/**
	Work only for AT+HTTPREAD=<n> nad AT+CIPRXGET=2,<n>
*/


int ResultParserStateBase::findLastIndexForRead(const char* str){
	int index = this->refBuffer.indexOf(str);
	if(index == -1)
		return -1;
	
	//set position after given str
	index += strlen(str);
	
	int endIndex = this->refBuffer.indexOfFrom(index, END_LINE);
	if(endIndex == -1)
		return -1;
	
	//set position on last character of end line
	endIndex += (strlen(END_LINE) - 1);
	
	//parse integer that follow the string
	int dataLength = atoi(&this->refBuffer[index]);
	
	return endIndex + dataLength;
}



const char* ResultParserStateBase::routeReadType(READ_TYPE type){
	switch(type){
		case READ_TYPE_HTTP:
			return HTTP_HTTPREAD_ANWSER;
		
		case READ_TYPE_TCP:
			return TCP_READ_ANWSER;
	}
	
	return nullptr;	//won't happen
}


/**
	MUST BE OVERRIDED BY SUBCLASSES
*/


int ResultParserStateBase::getAmountToDeleteAfterRead(){
	return 0;
}


/**
	MUST BE OVERRIDED BY SUBCLASSES
*/


bool ResultParserStateBase::isReadEnded(int index){
	return false;
}


//TEXT VERSION


bool ResultParserStateText::isSimpleMessageReady(){
	if(this->checkError())
		return true;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;

	
	return false;
}



bool ResultParserStateText::isComplexMessageReady(){
	return isSimpleMessageReady();
}



bool ResultParserStateText::removeResultCode(){
	refBuffer.remove(TEXT_SUCCESS);
	return true;
}


int ResultParserStateText::fetchResultCode(){
	if(this->checkError())
		return ANWSER_CODES::ERROR;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;

	
	return ANWSER_CODES::UNDEFINED;
}



int ResultParserStateText::getAmountToDeleteAfterRead(){
	return strlen(TEXT_SUCCESS);
}



bool ResultParserStateText::isReadEnded(int index){
	return this->refBuffer.indexOfFrom(index, TEXT_SUCCESS);
}




//NUMBER VERSION


bool ResultParserStateDigit::isSimpleMessageReady(){
	if(this->checkError())
		return true;

	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;


	return false;
}



bool ResultParserStateDigit::isComplexMessageReady(){
	if(this->refBuffer.getLength() <= 3){
		return false;
	}
	
	if(this->checkError())
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_COMPLEX_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_COMPLEX_ERROR) != -1)
		return true;
	
	
	return false;
}


bool ResultParserStateDigit::removeResultCode(){
	refBuffer.remove(DIGIT_COMPLEX_SUCCESS);
	return true;
}


int ResultParserStateDigit::fetchResultCode(){
	if(this->checkError())
		return ANWSER_CODES::ERROR;

	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;


	return ANWSER_CODES::UNDEFINED;
}



int ResultParserStateDigit::getAmountToDeleteAfterRead(){
	return strlen(DIGIT_SUCCESS);
}



bool ResultParserStateDigit::isReadEnded(int index){
	return this->refBuffer.indexOfFrom(index, DIGIT_SUCCESS);
}
