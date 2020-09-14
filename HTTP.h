#pragma once
#include "AbstractAction.h"
#include "GPRS.h"
#include "SimHandler.h"

enum HTTP_state : int
{
	idle = 0,	//have no task
	waitingForGPRS = 1,
	readyToWork = 2,
	sessionInitiated = 3,
	URLsetted = 4,
	ContentSetted = 5,
	readyToFeedData = 6,
	dataSetted = 7,
	dataSended = 8,
	responceHeaderReaded = 9,
	dataRead = 10,
	closingGPRS = 11
};

class HTTP : public AbstractAction
{
	public:
		bool init(SimHandler* pHandler);
		
		//main purpose is to send post data to server
		int doActions(unsigned long deltaTime) override;
		void setDataToSend(const String& data);

	private:
		bool isGPRS_Open(unsigned long deltaTime);
		String getPostDataString();
	
		GPRS gprs;
		SimHandler* pSimHandler;
		
		//After succeed sending will be set to ""
		String dataToSend;
		
		int currentState = idle;
};
