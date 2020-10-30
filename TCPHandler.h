#pragma once

#include "Enums.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "TCPReader.h"
#include "Buffer/FixedBuffer.h"
#include "SimState.h"
#include "LongCommandHandler.h"



template<int N>
class TCPHandler : public LongCommandHandler
{
	public:
		TCPHandler(
				SimCommandPort& simPort,
				SimResultParser<N>& parser,
				ParameterHandler& parameters,
				SimState& state
				);
		
		
		bool handle() override;
		

		bool connect();
		TCPReader<N> readMessage(FixedBuffer<N> buffer);
		
	private:
		bool handleInitial();
		bool handleIpStart();
		bool handleGPRSAct();
		bool handleIpStatus();
		bool handlePDPDeact();
		TCP_STATE handleUndefinied();
		
		
		SimCommandPort& refPort;
		SimResultParser<N>& refParser;
		ParameterHandler& refParameters;
		SimState& refState;
		
		bool isLastCommandCIICR;
};


template class TCPHandler<FIXED_BUFFER_SIZE>;