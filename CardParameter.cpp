#include "CardParameter.h"

#include "Util.h"



CardParameter::CardParameter(uint8_t* pUid, uint16_t uidLength){
	for(unsigned int i = 0; i < uidLength; i++){
		uid[i] = pUid[i];
	}
	
	this->uidLength = uidLength;
}


CardParameter& CardParameter::operator=(const CardParameter& ref){
	for(unsigned int i = 0; i < ref.uidLength; i++){
		this->uid[i] = ref.uid[i];
	}
	
	this->uidLength = ref.uidLength;
	
	return *this;
}


int CardParameter::getLength(){
	int commas = uidLength - 1;
	int numbers = 0;
	for(unsigned int i = 0; i < uidLength; i++){
		numbers += findLongLength(uid[i]);
	}
	
	return numbers + commas;
}


void CardParameter::handleWritingValue(BaseWriter& writer){
	for(unsigned int i = 0; i < uidLength; i++){
		writer.write(uid[i]);
		//don't append extra comma
		if(i == uidLength - 1){
			break;
		}
		
		writer.write(',');
	}
}