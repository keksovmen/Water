#pragma once

#include <Arduino.h>
#include "BaseParameter.h"


/**
	ATTENTION!
	Uses String that implemented through dynamic memory
	so be aware of your spare memory
*/

class StringParameter : public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		void parse(const char* str) override;
		
		void setValue(const char* str);
		const char* getValue();
	private:
		String value;
};