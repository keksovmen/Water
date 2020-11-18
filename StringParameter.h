#pragma once

#include <Arduino.h>
#include "BaseParameter.h"
#include "Buffer/FixedBuffer.h"


/**
	ATTENTION!
	Uses String that implemented through dynamic memory
	so be aware of your spare memory
*/

template<int N>
class StringParameter : public BaseParameter
{
	public:
		int getLength() override {return value.getLength();}
		
		void handleWritingValue(BaseWriter& writer) override {
			writer.write(value.begin());
		}
		
		//здеся копирует до конца а надо до $
		void parse(const char* str) override {
			int length = 0;
			for(const char* p = str; *p != '\0'; p++){
				if((*p) == (*ENTRY_ENDING)){
					break;
				}
				length++;
			}
			
			if(length > value.getSize()){
				Serial.println("String Param Length > N");
				length = value.getSize();
			}
			
			for(int i = 0; i < length; i++){
				value[i] = str[i];
			}
			value[length] = '\0';
			// value = str;
		}
		
		// void setValue(const char* str);
		const char* getValue(){return value.begin();}
	private:
		FixedBuffer<N> value;
		// String value;
};