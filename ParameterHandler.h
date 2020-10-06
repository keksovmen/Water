#pragma once

#include "Parameter.h"
#include "Enums.h"

class ParameterHandler
{
	public:
		
		int getLength();
		void handleWritingValue(BaseWriter& writer);
		
		Parameter<PrimitivFloatParameter<double>>& getTemp(){return temp;}
		Parameter<PrimitivFloatParameter<double>>& getPressure(){return press;}
		// Parameter<Clock>& getClock(){return clock;}
		
	private:
		Parameter<PrimitivFloatParameter<double>> temp =
			Parameter<PrimitivFloatParameter<double>>(0);
		
		Parameter<PrimitivFloatParameter<double>> press = 
			Parameter<PrimitivFloatParameter<double>>(1);
		// Parameter<Clock> clock = Parameter<Clock>(PARAMETER_TYPES::PARAMETER_USER_DEFINED, 2);
	
};