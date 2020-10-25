#pragma once

#include <IPAddress.h>
#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "Constants.h"


/**
	Simplifies implementation of 
	SimHandler::sendPost or sendGet methods
	
	Current state is requries inmplementation
	into SimHandler class
*/


template<int N>
class HTTPHandler
{
	public:
		HTTPHandler(	SimCommandPort& simPort,
						SimResultParser<N>& parser
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
		
		bool initGetRequest();
	
	private:
		bool initSession();
		bool setPostURL(IPAddress& address, const char* url);
		void setGetURL();
		bool setContentForPHP();
		bool startDataTransmition(int dataLength);
		bool terminateSession();
		
		
		SimCommandPort& refPort;
		SimResultParser<N>& refParser;
	
};


template class HTTPHandler<FIXED_BUFFER_SIZE>;