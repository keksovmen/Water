#pragma once

#include <SoftwareSerial.h>
#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"
#include "Enums.h"
#include "GPRSHandler.h"
#include "PostDataHandler.h"
#include "GetDataHandler.h"

template<int N>
class SimHandler
{
	public:
		SimHandler(SoftwareSerial& refPort);
		
		bool isModuleUp();
		bool isModuleAlive();
		NETWORK_CONNECTION isConnectedToNetwork();
		bool setDefaultParams();
		bool connectToGPRS(const char* apn);
		bool disconnectFromGPRS();
		PostDataHandler<N> sendPostRequest(const char* url, int dataLength);
		GetDataHandler<N> sendGetRequest();
		
	private:
		SimIOWrapper<N> wrapper;
		SimCommandWriter<N> writer;
		SimResultParser<N> parser;
		GPRSHandler<N> gprsHandler;
		
};

template class SimHandler<128>;



