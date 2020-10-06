#pragma once

#include "Parameter.h"
#include "Enums.h"

class ParameterHandler
{
	public:
		
		int getLength();
		void handleWritingValue(BaseWriter& writer);
		
		Parameter<double>& getTemp(){return temp;}
		Parameter<double>& getPressure(){return press;}
		
	private:
		Parameter<double> temp = Parameter<double>(PARAMETER_TYPES::PARAMETER_DOUBLE, 0);
		Parameter<double> press = Parameter<double>(PARAMETER_TYPES::PARAMETER_DOUBLE, 1);
	
};