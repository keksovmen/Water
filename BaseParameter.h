#pragma once

#include "BaseWriter.h"

class BaseParameter
{
	public:
		virtual int getLength() = 0;
		virtual void handleWritingValue(BaseWriter& writer) = 0;
};



/**
	@param T int, long
*/

template<typename T>
class PrimitivIntParameter : public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		T& getValue(){return value;}
	
	private:
		T value;
};


/**
	@param T float, double
*/

template<typename T>
class PrimitivFloatParameter : public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		T& getValue(){return value;}
	
	private:
		T value;
};


template class PrimitivFloatParameter<double>;
template class PrimitivIntParameter<int>;