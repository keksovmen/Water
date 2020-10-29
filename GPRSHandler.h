#pragma once

#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "Constants.h"
#include "SimState.h"
#include "LongCommandHandler.h"


/**
	Handles GPRS connection disconnection

*/

template<int N>
class GPRSHandler : public LongCommandHandler
{
	public:
		GPRSHandler(	SimCommandPort& simPort,
						SimResultParser<N>& parser,
						SimState& state
						);
		
		
		bool handle() override;
		
		/**
			Check if module is connected to network
			
			//TODO: made instead of bool use enum,
			//there is CONNECTING status that can fail
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
		
		bool close();
		
		
	private:
	
		//short cut for connect() method
		int retriveStatus();
	
		SimCommandPort& refPort;
		SimResultParser<N>& refParser;
		SimState& refState;
		
		
};

template class GPRSHandler<FIXED_BUFFER_SIZE>;