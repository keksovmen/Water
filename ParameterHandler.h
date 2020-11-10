#pragma once

#include "Parameter.h"
#include "Enums.h"
#include "Clock.h"
#include "CardParameter.h"
#include "IPAddressParam.h"
#include "StringParameter.h"


class ParameterHandler
{
	public:
		
		int getLength();
		void handleWritingValue(BaseWriter& writer);
		
		Parameter<PrimitivFloatParameter<double>>& getTemp(){return temp;}
		Parameter<PrimitivFloatParameter<double>>& getPressure(){return press;}
		
		Parameter<Clock>& getClock(){return clock;}
		Parameter<IPAddressParam>& getAddress(){return address;}
		
		Parameter<PrimitivIntParameter<int>>& getUserVolume(){return userVolume;}
		Parameter<PrimitivIntParameter<int>>& getGivenVolume(){return givenVolume;}
		
		Parameter<CardParameter>& getCard(){return card;}
		Parameter<StringParameter>& getApn(){return apn;}
		
	private:
		Parameter<PrimitivFloatParameter<double>> temp =
							Parameter<PrimitivFloatParameter<double>>(0);
		
		Parameter<PrimitivFloatParameter<double>> press = 
							Parameter<PrimitivFloatParameter<double>>(1);
			
		Parameter<Clock> clock = Parameter<Clock>(2);
		
		Parameter<IPAddressParam> address = Parameter<IPAddressParam>(3);
		
		Parameter<PrimitivIntParameter<int>> userVolume = 
								Parameter<PrimitivIntParameter<int>>(4);
		Parameter<PrimitivIntParameter<int>> givenVolume = 
								Parameter<PrimitivIntParameter<int>>(5);
	
		Parameter<CardParameter> card = Parameter<CardParameter>(6);
		
		Parameter<StringParameter> apn = Parameter<StringParameter>(7);
};