#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"

template<int N>
class PostDataHandler
{
	public:
		PostDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer);
		
		void write(const char* str);
		bool send();
		bool isSended();
		bool isSendedSuccesfully();
		void finish();
		
	private:
		SimIOWrapper<N>& refWrapper;
		SimResultParser<N>& refParser;
		SimCommandWriter<N>& refWriter;
		
};

template class PostDataHandler<128>;
