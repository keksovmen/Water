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
		
		int fetchResultCode();
		int fetchSimpleTextCode();
		int fetchNetworkRegistration();
		int fetchGPRSStatus();
		int fetchHTTPStatus();
		
	private:
		FixedBuffer<N>& buffer;
		
};

template class SimResultParser<128>;
// #include "SimResultParser.cpp"
