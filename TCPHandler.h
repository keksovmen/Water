#pragma once

#include "Enums.h"
#include "SimTools.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "TCPReader.h"
#include "Buffer/FixedBufferBase.h"
#include "LongCommandHandler.h"



class TCPHandler : public LongCommandHandler
{
	public:
		TCPHandler(
				SimTools& tools,
				ParameterHandler& parameters
				);
		
		
		bool handle() override;
		

		bool connect();
		TCPReader readMessage(FixedBufferBase& buffer);
		void sendPong();
		
	private:
		bool handleInitial();
		bool handleIpStart();
		bool handleGPRSAct();
		bool handleIpStatus();
		bool handlePDPDeact();
		TCP_STATE handleUndefinied();
		
		
		SimTools& refTools;
		ParameterHandler& refParameters;
		
		bool isLastCommandCIICR;
};