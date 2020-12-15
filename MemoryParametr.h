#pragma once

#include <EEPROM.h>
#include "Parameter.h"


template<typename T>
class MemoryParametr : public Parameter<T>
{
	public:
		MemoryParametr(int id, int address) : 
			Parameter<T>(id), address(address)
		{
			EEPROM.get(address, Parameter<T>::getValue());
		}
		
		void parse(const char* str) override{
			Parameter<T>::parse(str);
			EEPROM.put(address, Parameter<T>::getValue());
		}
		
	private:
		const int address;
	
	
};