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
bool ResultParserStateBase<N>::isReadHttpMessageFull(){
	int lastIndexForRead = findLastIndexForRead();
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
	int index = this->refBuffer.indexOfEnd("+CME ERROR: ");
	if(this->refBuffer.indexOfFrom(index, END_LINE) != -1){
		lastErrorCode = atoi(this->refBuffer[index + 11]);
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
void ResultParserStateBase<N>::removeReadHttpGarbage(){
	int index = this->refBuffer.indexOf("\r\n+HTTPREAD: ");
	int endIndex = this->refBuffer.indexOfFrom(index + 12, END_LINE);
	//set on first character of real data
	endIndex += 2;
	
	//remove from right to left cause everything will fuck up 
	//the other way around!
	this->refBuffer.remove(findLastIndexForRead()+ 1, 
					getAmountToDeleteAfterRead());
					
	this->refBuffer.remove(index, endIndex - index);
	
}


template<int N>
int ResultParserStateBase<N>::findLastIndexForRead(){
	int index = this->refBuffer.indexOf("\r\n+HTTPREAD: ");
	if(index == -1)
		return -1;
	
	//set position on to space character
	index += 12;
	
	int endIndex = this->refBuffer.indexOfFrom(index, END_LINE);
	if(endIndex == -1)
		return -1;
	
	//set position on last character of end line
	endIndex += (strlen(END_LINE) - 1);
	
	int dataLength = atoi(&this->refBuffer[index]);
	
	return endIndex + dataLength;
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
