#include "TemperatureSensor.h"
#include <Arduino.h>


void TemperatureSensor::doActivity(){
	if(!isInit){
		isInit = init();
		if(!isInit) return;
	}
	
	if(!isAsked){
		askTemp();
		isAsked = true;
		return;
	}
	
	if(timer.isOpen()){
		isAsked = false;
		
		if(!setTemp()){
			isInit = false;
		}
	}
	
	//1 - if not connected? try to init()
	//2 - if init() ask for temperature
	//3 - wait for temperature
	//4 - if temp is eeror go to init again
}


bool TemperatureSensor::init(){
	sensor.begin();
	if(sensor.getDS18Count() == 0){
		return false;
	}
	
	sensor.setResolution(12);
	sensor.setWaitForConversion(false);
	
	return true;
}

void TemperatureSensor::askTemp(){
	sensor.requestTemperatures();
	unsigned long delay = 750 / (1 << (12 - sensor.getResolution()));
	timer.sheduleDelay(delay);
}

bool TemperatureSensor::setTemp(){
	temperature = sensor.getTempCByIndex(0);
	return temperature != DEVICE_DISCONNECTED_C;
}