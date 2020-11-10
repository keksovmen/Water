#pragma once


#include "BaseParameter.h"
#include "Constants.h"



class ParameterWriter : BaseParameter
{
	public:
		bool add(BaseParameter* param);
		
		
		int getLength() override;
		
		void handleWritingValue(BaseWriter& writer) override;
		
		
		
	private:
		BaseParameter* params[SIZE_OF_PARAMETER_WRITER];
		int index = 0;
	
};