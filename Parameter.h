#pragma once

#include "BaseParameter.h"
#include "Enums.h"
#include "Util.h"



template<typename T>
class Parameter : public BaseParameter
{
	public:
		Parameter(int id) : id(id){}
		
		
		/**
			@return length in characters: id + value
		*/
		
		int getLength() override{
			int idLength = findLongLength(id);
			//+ 1 from = sign between them
			return idLength + value.getLength() + 1;
		}
		
		
		void handleWritingValue(BaseWriter& writer) override{
			writer.write(id);
			writer.write('=');
			value.handleWritingValue(writer);
		}
		
		
		void parse(const char* str) override {
			value.parse(str);
		};
		
		int getId(){return id;}
		T& getValue(){return value;}
		
	private:
		const int id;
		
		T value;
		
};


// template class Parameter<int>;
// template class Parameter<PrimitivFloatParameter<double>>;
// template class Parameter<Clock>;
// template class Parameter<IPAddressParam>;
// template class Parameter<PrimitivIntParameter<int>>;
// template class Parameter<CardParameter>;
// template class Parameter<Clock>;
// template class Parameter<double>;