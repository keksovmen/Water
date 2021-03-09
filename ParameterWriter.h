#pragma once


#include "BaseParameter.h"
#include "Constants.h"


template <int N>
class ParameterWriter : BaseParameter
{
	public:
		bool add(BaseParameter* param){
			if (index >= N){
				return false;
			}
	
			params[index] = param;
			index++;
	
			return true;
		}
		
		
		int getLength() override {
			int length = 0;
			for(int i = 0; i < index; i++){
				length += params[i]->getLength();
			}
			
			length += index - 1;
			
			return length;
		}
		
		void handleWritingValue(BaseWriter& writer) override {
			for(int i = 0; i < index; i++){
				params[i]->handleWritingValue(writer);
				if(i < index - 1){
					writer.write('&');
				}
			}
		}
		
		void parse(const char* str) override {
			//don't to anything
		}
		
		
	private:
		BaseParameter* params[N];
		int index = 0;
	
};