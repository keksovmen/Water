#pragma once
#include "Buffer/FixedBuffer.h"

template<int N>
class SimResultParser
{
	public:
		explicit SimResultParser(FixedBuffer<N>& refBuffer);
		
		bool isSimpleMessageReady();
		bool isComplexMessageReady();
		bool containDownload();
		bool isHttpActionPresents();
		
		int fetchResultCode();
		int fetchSimpleTextCode();
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
