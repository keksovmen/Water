#pragma once

#include <stdint.h>
#include "BaseParameter.h"



class CardParameter : public BaseParameter
{
	public:
		
		CardParameter(){};
		CardParameter(uint8_t* pUid, uint16_t uidLength);
		
		CardParameter& operator=(const CardParameter& ref);
		
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		
		
	private:
		//be aware of possible larger lengths
		uint8_t uid[7];
		uint16_t uidLength;
	
	
	
};