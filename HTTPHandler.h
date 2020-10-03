#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"

template<int N>
class HTTPHandler
{
	public:
		HTTPHandler(SimIOWrapper<N>& refWrapper,SimCommandWriter<N>& refWriter, SimResultParser<N>& refParser);
		
		bool initPostRequest(const char* url, int dataLength);
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