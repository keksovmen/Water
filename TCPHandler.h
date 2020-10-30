#pragma once

#include "Enums.h"
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "TCPReader.h"
#include "Buffer/FixedBuffer.h"



template<int N>
class TCPHandler
{
	public:
		TCPHandler(
				SimCommandPort& simPort,
				SimResultParser<N>& parser,
				ParameterHandler& parameters
				);
		
		
		bool isConnected();
		bool connect();
		
		/**
			Call when module accidently dies from power lose
			Changes state to CLOSED
		*/
		
		void reset();
		void connectedSuccessfully();
		void connectionFaild();
		void shutOk();
		void closedConnection();
		
		void updateState();
		
		void incomingMessage();
		void clearMessage();
		
		TCPReader<N> readMessage(FixedBuffer<N> buffer);
		
		
		bool isMessageWaiting(){return hasMessage;}
		
	private:
		bool writeDefaults();
		bool setAPN();
		bool connectToGPRS();
		bool getMyIp();
		bool connecToServer();
		bool tryToShutConenction();
		bool askStatus();
		void tryUpdateState();
		
	
		TCP_STATE state = TCP_STATE::TCP_STATE_WRITING_DEFAULTS;
		unsigned long timeStartOfCGATT;
		bool hasMessage = false;
		
		SimCommandPort& refPort;
		SimResultParser<N>& refParser;
		ParameterHandler& refParameters;
};


template class TCPHandler<FIXED_BUFFER_SIZE>;