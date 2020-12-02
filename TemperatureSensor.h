#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>
#include "Activity.h"
#include "TimeHandler.h"



class TemperatureSensor : public Activity
{
	public:
		TemperatureSensor(int sensorPin) : sensorPin(sensorPin){
			oneWire = OneWire(sensorPin);
			sensor = DallasTemperature(&oneWire);
		}
		
		//Mesures temperature here
		void doActivity() override;
		
		double getTemperature(){return temperature;}
		
	private:
		bool init();
		void askTemp();
		bool setTemp();
		
	
	
		const int sensorPin;
		
		OneWire oneWire;
		DallasTemperature sensor;
		TimeHandler timer;
	
		double temperature = DEVICE_DISCONNECTED_C;
		bool isInit = false;
		bool isAsked = false;
		
		
};
