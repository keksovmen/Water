#pragma once

#include "BaseReader.h"
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
		HTTPHandler(BaseReader& reader, 
						SimCommandWriter& writer, 
						SimResultParser<N>& parser);
		
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
		
		BaseReader& refReader;
		SimCommandWriter& refWriter;
		SimResultParser<N>& refParser;
	
};


template class HTTPHandler<128>;