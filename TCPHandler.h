#pragma once

#include "Enums.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "TCPReader.h"
#include "Buffer/FixedBufferBase.h"
#include "SimState.h"
#include "LongCommandHandler.h"



class TCPHandler : public LongCommandHandler
{
	public:
		TCPHandler(
				SimCommandPort& simPort,
				SimResultParser& parser,
				ParameterHandler& parameters,
				SimState& state
				);
		
		
		bool handle() override;
		

		bool connect();
		TCPReader readMessage(FixedBufferBase& buffer);
		
	private:
		bool handleInitial();
		bool handleIpStart();
		bool handleGPRSAct();
		bool handleIpStatus();
		bool handlePDPDeact();
		TCP_STATE handleUndefinied();
		
		
		SimCommandPort& refPort;
		SimResultParser& refParser;
		ParameterHandler& refParameters;
		SimState& refState;
		
		bool isLastCommandCIICR;
};