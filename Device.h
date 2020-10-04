#pragma once
#include <Wire.h>
#include <SFE_BMP180.h>


/*	Connections for PINS
		A5 - SCL
		A4 - SDA
		5V - ViN
		GND - GND
*/
class Device
{
	public:
		Device();
		bool init();
	
		void readResults();
	
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