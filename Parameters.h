#pragma once

#include "BaseWriter.h"

class Parameter
{
	public:
		explicit Parameter(int id);
		
		int getValueLength() = 0;
		void handleWritingValue(BaseWriter& writer) = 0;
		
	private:
		const int id;
		
};


class ParameterDouble : public Parameter
{
	public:
		explicit ParameterDouble(int id);
	
		int getValueLength() override;
		void handleWritingValue(BaseWriter& writer) override;
};


class ParameterInteger : public Parameter
{
	public:
		explicit ParameterInteger(int id);
	
		int getValueLength() override;
		void handleWritingValue(BaseWriter& writer) override;
};


class ParameterClock : public Parameter
{
	public:
		explicit ParameterClock(int id);
	
		int getValueLength() override;
		void handleWritingValue(BaseWriter& writer) override;
}