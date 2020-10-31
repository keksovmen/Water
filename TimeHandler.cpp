#include "TimeHandler.h"

#include <Arduino.h>



void TimeHandler::sheduleDelay(unsigned long duration){
	isSheduled = true;
	delayDuration = duration;
	startTime = millis();
}


bool TimeHandler::isOpen(){
	if(!isSheduled){
		return true;
	}
	
	if(millis() - startTime >= delayDuration){
		isSheduled = false;
		return true;
	}
	
	return false;
}