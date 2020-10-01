#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"


template<int N>
class DataHandler
{
	public:
		DataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer);
		
		virtual bool send() = 0;
		
		
		void write(const char* str);
		void write(char c);
		void write(int val);
		
		bool isSended();
		bool isSendedSuccesfully();
		
		void finish();
		
		bool readResponce();
		
		FixedBuffer<N> getBuffer(){ return refWrapper.getBuffer();};
		
	protected:
		SimIOWrapper<N>& refWrapper;
		SimResultParser<N>& refParser;
		SimCommandWriter<N>& refWriter;
		
		int readIndex = 0;
		
		unsigned long responceLength = 0;
		
	
};

template class DataHandler<128>;