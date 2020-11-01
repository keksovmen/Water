#pragma once

#include <IPAddress.h>
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "Constants.h"
#include "SimState.h"


/**
	Simplifies implementation of 
	SimHandler::sendPost or sendGet methods
	
	Current state is requries inmplementation
	into SimHandler class
*/


class HTTPHandler
{
	public:
		HTTPHandler(	SimCommandPort& simPort,
						SimResultParser& parser,
						SimState& state
						);
		
		/**
			Tries to initiate POST session
			
			@return true if succeed
		*/
		
		bool initPostRequest(IPAddress& address, const char* url, int dataLength);
		
		
		/**
			Tries to initiate GET session
			
			@return true if succeed
		*/
		
		bool initGetRequest(IPAddress& address, const char* url);
	
	private:
		bool initSession();
		bool setPostURL(IPAddress& address, const char* url);
		void setGetURL(IPAddress& address, const char* url);
		bool setContentForPHP();
		bool startDataTransmition(int dataLength);
		bool terminateSession();
		
		
		SimCommandPort& refPort;
		SimResultParser& refParser;
		SimState& refState;
	
};