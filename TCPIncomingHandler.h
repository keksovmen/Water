#pragma once

#include "Buffer/FixedBufferBase.h"
#include "ParameterHandler.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "Constants.h"


class TCPIncomingHandler
{
	public:
	
		void handleMessage(
				FixedBufferBase& buffer, 
				ParameterHandler& params,
				SimCommandPort& simPort,
				SimResultParser& parser 
				);
		
	private:
		int findCommand(FixedBufferBase& buffer, int& returnIndex);
		void removeCommand(FixedBufferBase& buffer, int cmd, int index);
		void handleCommand(
						FixedBufferBase& buffer, 
						int cmd, 
						int index, 
						ParameterHandler& params,
						SimCommandPort& simPort,
						SimResultParser& parser);
		
		void handleClockUpdate(FixedBufferBase& buffer, int index, ParameterHandler& params);
		void handlePing(SimCommandPort& simPort, SimResultParser& parser, FixedBufferBase& buffer);
	
};