#include "AbstractAction.h"

void AbstractAction::setDelay(unsigned long delay){
	timeDelay = delay;
}

bool AbstractAction::checkIfTimePassed(unsigned long deltaTime){
	timeCounter += deltaTime;
	if(timeCounter > timeDelay){
		timeCounter = 0;
		return true;
	}
	return false;
}
