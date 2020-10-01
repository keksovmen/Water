#pragma once

#include <SoftwareSerial.h>
#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"
#include "Enums.h"
#include "GPRSHandler.h"
#include "PostDataHandler.h"

template<int N>
class SimFacade
{
	public:
		SimFacade(SoftwareSerial& refPort);
		
		bool isModuleUp();
		NETWORK_CONNECTION isConnectedToNetwork();
		bool setDefaultParams();
		bool connectToGPRS(const char* apn);
		bool disconnectFromGPRS();
		PostDataHandler<N> sendPostRequest(const char* url, int dataLength);
		
		
	private:
		SimIOWrapper<N> wrapper;
		SimCommandWriter<N> writer;
		SimResultParser<N> parser;
		GPRSHandler<N> gprsHandler;
		
};

template class SimFacade<128>;



