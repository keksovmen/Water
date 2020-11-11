#pragma once

#include "Buffer/FixedBufferBase.h"
#include "ParameterHandler.h"
#include "SimTools.h"
#include "Constants.h"
#include "Enums.h"


class TCPIncomingHandler
{
	public:
		TCPIncomingHandler(
							FixedBufferBase& buffer,
							ParameterHandler& params,
							SimTools& tools
							);
							
		void handleMessage();
		
	private:
		bool hasCommand();
		int getBeginingOfCommand();
		TCP_MESSAGE_TYPE getCommandType(int begining);
		void handleCommand(TCP_MESSAGE_TYPE cmd, int begining);
		void removeCommand(int begining);
		
		// int findCommand(int& returnIndex);
		// void removeCommand(int cmd, int index);
		// void handleCommand(int cmd, int index);
		
		// void handleClockUpdate(int index);
		void handleSetParam(int begining);
		void handlePing();
	
	
		FixedBufferBase& refBuffer;
		ParameterHandler& refParams;
		SimTools& refTools;
};