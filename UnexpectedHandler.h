#pragma once

#include "Buffer/FixedBufferBase.h"
#include "BaseReader.h"
#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "SimCommandWriter.h"
#include "TCPHandler.h"
#include "Constants.h"
#include "SimState.h"


/**
	Have to read data from SimIOWrapper,
	find something unexpected handle it,
	leave buffer without any trace of it
*/


class UnexpectedHandler : public BaseReader
{
	public:
		UnexpectedHandler(	FixedBufferBase& buffer,
							BaseReader& reader,
							SimState& state
							);
		
		
		void attachTCPHandler(TCPHandler* tcpHandler);
		
		
		//delegate to refReader
		bool read() override;
		bool readTimeout(unsigned long maxDelay) override;
		
		void handleSwitch();
		
		/**
			Represents if there was some unexpected messages
			such as: voltage, call, tcp incoming
			
			Digits as Power of 2, in binary form
		*/
		
		int unexpectedMessages = 0;
		
	private:
		void handleIncomingCall();
		
		bool readRemovingGarbage();
	
		FixedBufferBase& refBuffer;
		BaseReader& refReader;
		SimState& refState;
		TCPHandler* pTcpHandler;
		
		
};