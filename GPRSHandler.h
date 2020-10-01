#pragma once


#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"


template<int N>
class GPRSHandler
{
	public:
		GPRSHandler(SimIOWrapper<N>& wrapper, 
					SimCommandWriter<N>& writer,
					SimResultParser<N>& parser
					);
					
		bool isConnected();
		bool connect(const char* apn);
		bool close();
		
		
	private:
		int retriveStatus();
	
		SimIOWrapper<N>& refWrapper;
		SimCommandWriter<N>& refWriter;
		SimResultParser<N>& refParser;
		
		
};

template class GPRSHandler<128>;