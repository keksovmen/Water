#pragma once

#include "BaseWriter.h"
#include "Enums.h"


template<typename T>
class Parameter
{
	public:
		Parameter(PARAMETER_TYPES type, int id);
		
		
		/**
			@return length in characters: id + value
		*/
		
		int getLength();
		void handleWritingValue(BaseWriter& writer);
		
		int getId(){return id;}
		T& getValue(){return value;}
		
	private:
		const PARAMETER_TYPES type;
		const int id;
		
		T value;
		
};


// template class Parameter<int>;
template class Parameter<double>;
// template class Parameter<double>;