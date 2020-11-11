#pragma once

#include "BaseWriter.h"

class BaseParameter
{
	public:
		virtual int getLength() = 0;
		virtual void handleWritingValue(BaseWriter& writer) = 0;
		virtual void parse(const char* str) = 0;
};



/**
	@param T int, long
*/

// template<typename T>
class PrimitivIntParameter : public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		void parse(const char* str) override;
		long& getValue(){return value;}
	
	private:
		long value;
};


/**
	@param T float, double
*/

// template<typename T>
class PrimitivFloatParameter : public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
		void parse(const char* str) override;
		double& getValue(){return value;}
	
	private:
		double value;
};


// template class PrimitivFloatParameter<double>;
// template class PrimitivIntParameter<int>;