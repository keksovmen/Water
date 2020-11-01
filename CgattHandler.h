#pragma once

#include "LongCommandHandler.h"
#include "SimTools.h"
#include "Constants.h"


class CgattHandler : public LongCommandHandler
{
	public:
		CgattHandler(SimTools& tools);
				
		bool handle() override;
		
		
		/**
			
			@return true if after check status is connected
					false if after check status is disconnected
							and begin to connect
		*/
		
		bool connectToCGATT();
		
	private:
		bool askCGATTStatus();
		
		
		SimTools& refTools;
	
};