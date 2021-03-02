#pragma once

#include <EEPROM.h>
#include "Parameter.h"
#include "Constants.h"


/**
	Will save <T> in EEPROM
	Adress starts from {@link Constants.h}
*/


template<typename T>
class MemoryParametr : public Parameter<T>
{
	public:
		
	
		MemoryParametr(int id) : 
			Parameter<T>(id)
		{
			static int globalAdress = START_EEPROM_ADDRESS;
			address = globalAdress;
			globalAdress += sizeof(T);
			EEPROM.get(address, Parameter<T>::getValue());
		}
		
		void parse(const char* str) override{
			Parameter<T>::parse(str);
			EEPROM.put(address, Parameter<T>::getValue());
		}
		
	private:
		int address;
	
	
};
