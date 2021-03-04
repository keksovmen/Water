#pragma once

#include <Arduino.h>
#include "BaseParameter.h"
#include "Buffer/FixedBuffer.h"
#include "Constants.h"



template<int N>
class StringParameter : public BaseParameter
{
	public:
		int getLength() override {return value.getLength();}
		
		void handleWritingValue(BaseWriter& writer) override {
			writer.write(value.begin());
		}
		
		
		/**
			Copies until \0 or ENTRY_ENDING found
			or dedicated memory ends
		*/
		
		void parse(const char* str) override {
			int length = 0;
			const char* finding = strstr(str, ENTRY_ENDING);
			if(finding){
				length = finding - str;
			}else{
				length = strlen(str);
			}
			
			if(length > value.getSize()){
				Serial.println("String Param Length > N");
				length = value.getSize();
			}
			
			for(int i = 0; i < length; i++){
				value[i] = str[i];
			}
			value[length] = '\0';
		}
		
		const char* getValue(){return value.begin();}
	private:
		FixedBuffer<N> value;
};