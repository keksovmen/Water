#pragma once
#include "AbstractAction.h"
#include "SimHandler.h"


enum Start_state : int
{
	waitingForModule = 0,
	waitingForNetwork = 1,
	ready = 2
};

class StartHandler : public AbstractAction
{
	public:
		bool init(SimHandler* pHandler);
		
		virtual int doActions(unsigned long deltaTime) override;
		
		bool isReady() const;
		
		void close();
	
	private:
		bool parseNetworkRegistration();
	
		SimHandler* pSimHandler;
		
		int currentState = waitingForModule;
};
