#pragma once

#include "IPAddressParam.h"
#include "BaseParameter.h"
#include "Enums.h"
#include "Clock.h"
#include "CardParameter.h"



template<typename T>
class Parameter : public BaseParameter
{
	public:
		Parameter(int id);
		
		
		/**
			@return length in characters: id + value
		*/
		
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		
		int getId(){return id;}
		T& getValue(){return value;}
		
	private:
		const int id;
		
		T value;
		
};


// template class Parameter<int>;
template class Parameter<PrimitivFloatParameter<double>>;
template class Parameter<Clock>;
template class Parameter<IPAddressParam>;
template class Parameter<PrimitivIntParameter<int>>;
template class Parameter<CardParameter>;
// template class Parameter<Clock>;
// template class Parameter<double>;