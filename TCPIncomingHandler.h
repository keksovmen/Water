#pragma once

#include "Buffer/FixedBuffer.h"
#include "ParameterHandler.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "Constants.h"


template<int N>
class TCPIncomingHandler
{
	public:
	
		void handleMessage(
				FixedBuffer<N>& buffer, 
				ParameterHandler& params,
				SimCommandPort& simPort,
				SimResultParser<N>& parser 
				);
		
	private:
		int findCommand(FixedBuffer<N>& buffer, int& returnIndex);
		void removeCommand(FixedBuffer<N>& buffer, int cmd, int index);
		void handleCommand(
						FixedBuffer<N>& buffer, 
						int cmd, 
						int index, 
						ParameterHandler& params,
						SimCommandPort& simPort,
						SimResultParser<N>& parser);
		
		void handleClockUpdate(FixedBuffer<N>& buffer, int index, ParameterHandler& params);
		void handlePing(SimCommandPort& simPort, SimResultParser<N>& parser, FixedBuffer<N>& buffer);
	
};


template class TCPIncomingHandler<FIXED_BUFFER_SIZE>;