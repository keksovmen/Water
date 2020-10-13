#include "ResultParserState.h"
#include "StringData.h"
#include "Enums.h"



template<int N>
bool ResultParserStateBase<N>::isSimpleMessageReady(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return true;

	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;
	

	
	return false;
}


template<int N>
bool ResultParserStateBase<N>::isComplexMessageReady(FixedBuffer<N>& buffer){
	if(buffer.getLength() <= 3){
		return false;
	}
	
	if(checkError(buffer))
		return true;
	
	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_COMPLEX_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_COMPLEX_ERROR) != -1)
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
bool ResultParserStateBase<N>::isReadHttpMessageFull(FixedBuffer<N>& buffer){
	int lastIndexForRead = findLastIndexForRead(buffer);
	if(lastIndexForRead == -1){
		return false;
	}
	
	if(buffer.getLength() < lastIndexForRead + getAmountToDeleteAfterRead()){
		return false;
	}
	

	return isReadEnded(buffer, lastIndexForRead + 1);
 }
 
 
 template<int N>
 bool ResultParserStateBase<N>::checkError(FixedBuffer<N>& buffer){
	 if(buffer.indexOfFrom(buffer.indexOfEnd("+CME ERROR: "), END_LINE) != -1)
		return true;
	
	
	return false;
 }


template<int N>
int ResultParserStateBase<N>::fetchResultCode(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return ANWSER_CODES::ERROR;

	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;
	
	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;


	return ANWSER_CODES::UNDEFINED;	
}


/**
	NOT CALL FROM SUPER CLASS DUE TO LACK OF KNOLEGE
	OF WHAT TYPE OF MESSAGE DEAL WITH
*/

template<int N>
void ResultParserStateBase<N>::removeReadHttpGarbage(FixedBuffer<N>& buffer){
	int index = buffer.indexOf("\r\n+HTTPREAD: ");
	int endIndex = buffer.indexOfFrom(index + 12, END_LINE);
	//set on first character of real data
	endIndex += 2;
	
	//remove from right to left cause everything will fuck up 
	//the other way around!
	buffer.remove(findLastIndexForRead(buffer)+ 1, 
					getAmountToDeleteAfterRead());
					
	buffer.remove(index, endIndex - index);
	
}


template<int N>
int ResultParserStateBase<N>::findLastIndexForRead(FixedBuffer<N>& buffer){
	int index = buffer.indexOf("\r\n+HTTPREAD: ");
	if(index == -1)
		return -1;
	
	//set position on to space character
	index += 12;
	
	int endIndex = buffer.indexOfFrom(index, END_LINE);
	if(endIndex == -1)
		return -1;
	
	//set position on last character of end line
	endIndex += (strlen(END_LINE) - 1);
	
	int dataLength = atoi(&buffer[index]);
	
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
bool ResultParserStateBase<N>::isReadEnded(FixedBuffer<N>& buffer, int index){
	return false;
}

//TEXT VERSION

template<int N>
bool ResultParserStateText<N>::isSimpleMessageReady(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return true;

	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return true;

	
	return false;
}


template<int N>
bool ResultParserStateText<N>::isComplexMessageReady(FixedBuffer<N>& buffer){
	return isSimpleMessageReady(buffer);
}


template<int N>
int ResultParserStateText<N>::fetchResultCode(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return ANWSER_CODES::ERROR;

	if(buffer.indexOfEnd(TEXT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(TEXT_ERROR) != -1)
		return ANWSER_CODES::ERROR;

	
	return ANWSER_CODES::UNDEFINED;
}


template<int N>
int ResultParserStateText<N>::getAmountToDeleteAfterRead(){
	return strlen(TEXT_SUCCESS);
}


template<int N>
bool ResultParserStateText<N>::isReadEnded(FixedBuffer<N>& buffer, int index){
	return buffer.indexOfFrom(index, TEXT_SUCCESS);
}




//NUMBER VERSION

template<int N>
bool ResultParserStateDigit<N>::isSimpleMessageReady(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return true;

	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return true;


	return false;
}


template<int N>
bool ResultParserStateDigit<N>::isComplexMessageReady(FixedBuffer<N>& buffer){
	if(buffer.getLength() <= 3){
		return false;
	}
	
	if(checkError(buffer))
		return true;
	
	if(buffer.indexOfEnd(DIGIT_COMPLEX_SUCCESS) != -1)
		return true;
	
	if(buffer.indexOfEnd(DIGIT_COMPLEX_ERROR) != -1)
		return true;
	
	
	return false;
}


template<int N>
int ResultParserStateDigit<N>::fetchResultCode(FixedBuffer<N>& buffer){
	if(checkError(buffer))
		return ANWSER_CODES::ERROR;

	if(buffer.indexOfEnd(DIGIT_SUCCESS) != -1)
		return ANWSER_CODES::OK;
	
	if(buffer.indexOfEnd(DIGIT_ERROR) != -1)
		return ANWSER_CODES::ERROR;


	return ANWSER_CODES::UNDEFINED;
}


template<int N>
int ResultParserStateDigit<N>::getAmountToDeleteAfterRead(){
	return strlen(DIGIT_SUCCESS);
}


template<int N>
bool ResultParserStateDigit<N>::isReadEnded(FixedBuffer<N>& buffer, int index){
	return buffer.indexOfFrom(index, DIGIT_SUCCESS);
}
