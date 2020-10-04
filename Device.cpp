#include "Device.h"

Device::Device(){
}

bool Device::init(){
	bool readyState = false;
	if(!sensor.begin()){
		readyState = false;
	}else{
		readyState = true;
	}
	return readyState;
}

void Device::readResults(){
	// do actions, take mesures
	temperature = fetchTemperature();
	pressure = fetchPressure(temperature);
}

double Device::fetchTemperature(){
	double temperature;
	char status = sensor.startTemperature();
	if(status != 0){
		delay(status);
		
		status = sensor.getTemperature(temperature);
		if(status !=0){
			return temperature;
		}
	}
	return -1;
}

double Device::fetchPressure(double temperature){
	double pressure;
	char status = sensor.startPressure(3);
	if(status != 0){
		delay(status);
		
		status = sensor.getPressure(pressure, temperature);
		if(status != 0){
			return pressure;
		}
	}
	return -1;
}
