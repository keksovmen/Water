#pragma once

#include "Enums.h"
#include "SimTools.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "TCPReader.h"
#include "Buffer/FixedBufferBase.h"
#include "LongCommandHandler.h"
#include "UnexpectedHandler.h"



class TCPHandler : public LongCommandHandler, public UnexpectedHandler
{
	public:
		TCPHandler(
				SimTools& tools,
				ParameterHandler& parameters
				);
		
		
		bool handle() override;
		void handleUnexpected(FixedBufferBase& refBuffer) override;

		bool connect();
		TCPReader readMessage(FixedBufferBase& buffer);
		void sendPong();
		void sendId();
		void sendAcknowledgment();
		
	private:
		bool handleInitial();
		bool handleIpStart();
		bool handleGPRSAct();
		bool handleIpStatus();
		bool handlePDPDeact();
		TCP_STATE handleUndefinied();
		
		bool initSending();
		
		
		SimTools& refTools;
		ParameterHandler& refParameters;
		
		bool isLastCommandCIICR;
};