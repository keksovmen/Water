#pragma once

#include "Enums.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"
#include "BaseReader.h"
#include "ParameterHandler.h"
#include "Constants.h"


template<int N>
class TCPHandler
{
	public:
		TCPHandler(
				SimCommandWriter& writer,
				SimResultParser<N>& parser,
				BaseReader& reader, 
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
		
		void updateState();
		
		
	private:
		bool writeDefaults();
		bool setAPN();
		bool connectToGPRS();
		bool getMyIp();
		bool connecToServer();
		
		bool connectToCGATT();
		int waitForCGATT();
		
		bool askCGATTStatus();
		bool askStatus();
		
	
		TCP_STATE state = TCP_STATE::TCP_STATE_WRITING_DEFAULTS;
		unsigned long timeStartOfCGATT;
		
		SimCommandWriter& refWriter;
		SimResultParser<N>& refParser;
		BaseReader& refReader;
		ParameterHandler& refParameters;
};


template class TCPHandler<FIXED_BUFFER_SIZE>;