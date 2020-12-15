#pragma once

#include "Parameter.h"
#include "Enums.h"
#include "Clock.h"
#include "CardParameter.h"
#include "IPAddressParam.h"
#include "StringParameter.h"
#include "MemoryParametr.h"


class ParameterHandler
{
	public:
		
		//TODO: made inheret from a class of parameter
		// int getLength();
		// void handleWritingValue(BaseWriter& writer);
		
		bool hasId(int id);
		//TODO: made inheret from a class of parameter
		void parse(int id, const char* str);
		
		Parameter<PrimitivFloatParameter>& getSensorTempUp(){return sensorTempUp;}
		Parameter<PrimitivFloatParameter>& getSensorTempDown(){return sensorTempDown;}
		
		Parameter<Clock>& getClock(){return clock;}
		Parameter<IPAddressParam>& getAddress(){return address;}
		
		Parameter<PrimitivIntParameter>& getUserVolume(){return userVolume;}
		Parameter<PrimitivIntParameter>& getGivenVolume(){return givenVolume;}
		
		Parameter<CardParameter>& getCard(){return card;}
		Parameter<StringParameter<50>>& getApn(){return apn;}
		
		Parameter<PrimitivFloatParameter>& getTempUp(){return tempUp;}
		Parameter<PrimitivFloatParameter>& getTempDown(){return tempDown;}
		
		Parameter<PrimitivIntParameter>& getPlateId(){return plateID;}
		
	private:
		Parameter<PrimitivFloatParameter> sensorTempUp =
							Parameter<PrimitivFloatParameter>(0);
		
		Parameter<PrimitivFloatParameter> sensorTempDown = 
							Parameter<PrimitivFloatParameter>(1);
			
		Parameter<Clock> clock = Parameter<Clock>(2);
		
		Parameter<IPAddressParam> address = Parameter<IPAddressParam>(3);
		
		Parameter<PrimitivIntParameter> userVolume = 
								Parameter<PrimitivIntParameter>(4);
		Parameter<PrimitivIntParameter> givenVolume = 
								Parameter<PrimitivIntParameter>(5);
	
		Parameter<CardParameter> card = Parameter<CardParameter>(6);
		
		Parameter<StringParameter<50>> apn = Parameter<StringParameter<50>>(7);
		
		Parameter<PrimitivFloatParameter> tempUp = 
							Parameter<PrimitivFloatParameter>(8);
		Parameter<PrimitivFloatParameter> tempDown = 
							Parameter<PrimitivFloatParameter>(9);
							
		Parameter<PrimitivIntParameter> plateID = 
							Parameter<PrimitivIntParameter>(10);
};