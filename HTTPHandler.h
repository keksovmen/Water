#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"


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
		HTTPHandler(SimIOWrapper<N>& refWrapper,SimCommandWriter<N>& refWriter, SimResultParser<N>& refParser);
		
		/**
			Tries to initiate POST session
			
			@return true if succeed
		*/
		
		bool initPostRequest(const char* url, int dataLength);
		
		
		/**
			Tries to initiate GET session
			
			@return true if succeed
		*/
		
		bool initGetRequest();
	
	private:
		bool initSession();
		bool setPostURL(const char* url);
		void setGetURL();
		bool setContentForPHP();
		bool startDataTransmition(int dataLength);
		bool terminateSession();
		
		SimIOWrapper<N>& wrapper;
		SimCommandWriter<N>& writer;
		SimResultParser<N>& parser;
	
};


template class HTTPHandler<128>;