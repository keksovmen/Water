#pragma once
#include "Buffer/FixedBuffer.h"


/**
	THE MAIN RULE:
	When you try to fetch some data,
	you MUST know it already there!
	Make sure through ready methods!
	
	
*/


template<int N>
class SimResultParser
{
	public:
		explicit SimResultParser(FixedBuffer<N>& refBuffer);
		
		bool isSimpleMessageReady();
		bool isComplexMessageReady();
		bool containDownload();
		bool isHttpActionPresents();
		bool isReadHttpMessageFull();
		
		int fetchResultCode();
		int fetchNetworkRegistration();
		int fetchGPRSStatus();
		int fetchHTTPStatus();
		
		unsigned long fetchHttpResponceLength();
		
		void removeReadHttpGarbage();
		
	private:
		FixedBuffer<N>& buffer;
		
};

template class SimResultParser<128>;
// #include "SimResultParser.cpp"
