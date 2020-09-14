#pragma once

class AbstractAction
{
	public:
		// virtual ~AbstractAction() = 0;
	
		//deltaTime is difference between calls, in millis
		//return is user defined int representing state of given class
		virtual int doActions(unsigned long deltaTime) = 0;
		
		//delay in millis
		void setDelay(unsigned long delay);
	
	protected:
	
		//if time has passed, then timeCounter = 0, and return true
		//if not, then timeCounter += deltaTime
		bool checkIfTimePassed(unsigned long deltaTime);
		
		//in milli seconds
		//You have to set it
		//default 10, so if you forget it won't be pow(2,31)
		unsigned long timeDelay = 10;
		
		//show how much time passed without dooing shit
		//timeCounter += deltaTime, main logic
		//if timeCounter > timeDelay, doAction and not skip it
		unsigned long timeCounter = 0;
};
