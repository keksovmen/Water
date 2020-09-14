#pragma once
#include <Wire.h>
#include <SFE_BMP180.h>
#include "AbstractAction.h"

/*	Connections for PINS
		A5 - SCL
		A4 - SDA
		5V - ViN
		GND - GND
*/
class Device : public AbstractAction
{
	public:
		Device();
		bool init();
	
		//deltaTime is difference between calls, in millis
		//will return 0 if skipped turn, 1 if acted -> consumed time
		int doActions(unsigned long deltaTime) override;
	
		double getTemperature(){ return temperature; };
		double getPressure(){ return pressure; };
	
	
	
	private:
		//will cause program stop for some period of time, to collect data
		double fetchTemperature();
		//will cause program stop for some period of time, to collect data
		double fetchPressure(double temperature);
	
		SFE_BMP180 sensor;
	
		//in Celcies
		double temperature;
	
		//in milli bar
		double pressure;
};