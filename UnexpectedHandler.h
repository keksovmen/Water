#pragma once

#include "Buffer/FixedBuffer.h"
#include "BaseReader.h"
#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "SimCommandWriter.h"
#include "TCPHandler.h"
#include "Constants.h"


/**
	Have to read data from SimIOWrapper,
	find something unexpected handle it,
	leave buffer without any trace of it
*/


template<int N>
class UnexpectedHandler : public BaseReader
{
	public:
		UnexpectedHandler(	FixedBuffer<N>& buffer,
							BaseReader& reader
							);
		
		
		void attachTCPHandler(TCPHandler<N>* tcpHandler);
		
		
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
	
		FixedBuffer<N>& refBuffer;
		BaseReader& refReader;
		TCPHandler<N>* tcpHandler;
		
		
};


template class UnexpectedHandler<FIXED_BUFFER_SIZE>;

