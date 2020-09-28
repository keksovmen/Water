#pragma once

#include <SoftwareSerial.h>
#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"

template<int N>
class SimFacade
{
	public:
		SimFacade(SoftwareSerial& refPort);
		
		bool isModuleUp();
		
	private:
		SimIOWrapper<N> wrapper;
		SimCommandWriter<N> writer;
		SimResultParser<N> parser;
		
};

template class SimFacade<128>;



