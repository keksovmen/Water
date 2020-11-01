#pragma once

#include "LongCommandHandler.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "SimState.h"
#include "Constants.h"


class CgattHandler : public LongCommandHandler
{
	public:
		CgattHandler(
				SimCommandPort& port, 
				SimResultParser& parser,
				SimState& state
				);
				
		bool handle() override;
		
		
		/**
			
			@return true if after check status is connected
					false if after check status is disconnected
							and begin to connect
		*/
		
		bool connectToCGATT();
		
	private:
		bool askCGATTStatus();
		
		
		SimCommandPort& refPort; 
		SimResultParser& refParser;
		SimState& refState;
	
};