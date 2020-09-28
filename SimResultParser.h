#pragma once
#include "Buffer/FixedBuffer.h"

template<int N>
class SimResultParser
{
	public:
		bool isSimpleMessageReady(FixedBuffer<N>& buffer);
		bool isComplexMessageReady(FixedBuffer<N>& buffer);
		int fetchResultCode(FixedBuffer<N>& buffer);
		
	private:
		
		
};

template class SimResultParser<128>;
// #include "SimResultParser.cpp"
