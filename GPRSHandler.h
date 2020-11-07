#pragma once

#include "SimTools.h"
#include "Constants.h"
#include "SimState.h"
#include "LongCommandHandler.h"


/**
	Handles GPRS connection disconnection

*/

class GPRSHandler : public LongCommandHandler
{
	public:
		GPRSHandler(SimTools& tools);
		
		
		bool handle() override;
		
		/**
			Check if module is connected to network
			
			@return network connection status
		*/	
		
		BEARER_STATUS isConnected();
		
		
		/**
			Tries to conenct to GPRS
			
			@pram apn ask provider for it
			@return true if successfully connected
		*/
		
		bool connect(const char* apn);
		
		
		/**
			Tries to close conenction
			
			@return true if closed
		*/
		
		void close();
		
		
	private:
	
		//short cut for connect() method
		int retriveStatus();
	
		SimTools& refTools;
		
		bool lastCommandOpen;
		
		
};