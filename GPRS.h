#pragma once
#include "AbstractAction.h"
#include "SimHandler.h"
// #include "StartHandler.h"

//main purpose is to open GPRS connection

enum GPRS_state : int
{
	closed = 0,
	readyToOpen = 1,
	open = 2,
	waitingForStartHandler = 3
};

class GPRS : public AbstractAction
{
	public:
		bool init(SimHandler* pHandler);
		
		//has to open GPRS connection, after this just skip turns
		//return state of connection
		int doActions(unsigned long deltaTime) override;
		int getState() const {return currentState;}
		int close();
		
	private:
		// StartHandler startHandler;
		SimHandler* pSimHandler;
		
		int currentState = closed;

};
