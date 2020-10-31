#include "ResultParserState.h"
#include "StringData.h"
#include "Enums.h"



template<int N>
bool ResultParserStateBase<N>::isSimpleMessageReady(){
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


template<int N>
bool ResultParserStateBase<N>::isComplexMessageReady(){
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
	NOT CALL FROM SUPER CLASS DUE TO LACK OF KNOLEGE
	OF WHAT TYPE OF MESSAGE DEAL WITH
	
	Subclass must provide correct SimpleMEssageREady
	you can't sa for sure is read message full or not
*/

template<int N>
bool ResultParserStateBase<N>::isReadMessageFull(READ_TYPE type){
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
 
 
 template<int N>
bool ResultParserStateBase<N>::checkError(){
	int index = this->refBuffer.indexOfEnd(TEXT_CME_ERROR);
	if(this->refBuffer.indexOfFrom(index, END_LINE) != -1){
		index += strlen(TEXT_CME_ERROR);
		lastErrorCode = atoi(&this->refBuffer[index]);
		
		return true;
	}
	
	lastErrorCode = -1;
	return false;
}


template<int N>
int ResultParserStateBase<N>::fetchResultCode(){
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

template<int N>
void ResultParserStateBase<N>::removeReadGarbage(READ_TYPE type){
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

template<int N>
int ResultParserStateBase<N>::findLastIndexForRead(const char* str){
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


template<int N>
const char* ResultParserStateBase<N>::routeReadType(READ_TYPE type){
	switch(type){
		case READ_TYPE_HTTP:
			return HTTPREAD_ANWSER;
		
		case READ_TYPE_TCP:
			return TCP_READ_ANWSER;
	}
	
	return nullptr;	//won't happen
}


/**
	MUST BE OVERRIDED BY SUBCLASSES
*/

template<int N>
int ResultParserStateBase<N>::getAmountToDeleteAfterRead(){
	return 0;
}


/**
	MUST BE OVERRIDED BY SUBCLASSES
*/

template<int N>
bool ResultParserStateBase<N>::isReadEnded(int index){
	return false;
}


//TEXT VERSION

template<int N>
bool ResultParserStateText<N>::isSimpleMessageReady(){
	if(this->checkError())
		return true;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;

	
	return false;
}


template<int N>
bool ResultParserStateText<N>::isComplexMessageReady(){
	return isSimpleMessageReady();
}


template<int N>
int ResultParserStateText<N>::fetchResultCode(){
	if(this->checkError())
		return ANWSER_CODES::ERROR;

	if(this->refBuffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;

	
	return ANWSER_CODES::UNDEFINED;
}


template<int N>
int ResultParserStateText<N>::getAmountToDeleteAfterRead(){
	return strlen(TEXT_SUCCESS);
}


template<int N>
bool ResultParserStateText<N>::isReadEnded(int index){
	return this->refBuffer.indexOfFrom(index, TEXT_SUCCESS);
}




//NUMBER VERSION

template<int N>
bool ResultParserStateDigit<N>::isSimpleMessageReady(){
	if(this->checkError())
		return true;

	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;


	return false;
}


template<int N>
bool ResultParserStateDigit<N>::isComplexMessageReady(){
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


template<int N>
int ResultParserStateDigit<N>::fetchResultCode(){
	if(this->checkError())
		return ANWSER_CODES::ERROR;

	if(this->refBuffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(this->refBuffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;


	return ANWSER_CODES::UNDEFINED;
}


template<int N>
int ResultParserStateDigit<N>::getAmountToDeleteAfterRead(){
	return strlen(DIGIT_SUCCESS);
}


template<int N>
bool ResultParserStateDigit<N>::isReadEnded(int index){
	return this->refBuffer.indexOfFrom(index, DIGIT_SUCCESS);
}
