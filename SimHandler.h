#pragma once

#include <Stream.h>
#include <IPAddress.h>
#include "Enums.h"
#include "Constants.h"
#include "ParameterHandler.h"
#include "Buffer/FixedBuffer.h"
#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "UnexpectedHandler.h"
#include "SimCommandPort.h"
#include "GPRSHandler.h"
#include "DataHandler.h"
#include "HTTPHandler.h"
#include "TCPHandler.h"



/**
	BOSS OF THIS GYM
	
	Handles the highest lvl of interaction with sim

*/


template<int N>
class SimHandler
{
	public:
		SimHandler(Stream& refPort, ParameterHandler& parameters);
		
		/**
			@return true if module can anwser
		*/
		
		bool isModuleUp();
		
		
		/**
			Take some time to understand stae of module
			
			@return true if module is properly working
		*/
		
		bool isModuleAlive();
		
		
		/**
			@return status of network connection
		*/
		
		NETWORK_CONNECTION isConnectedToNetwork();
		
		
		/**
			Sets default params such as:
			echo off, turn digit responce mode etc.
			
			@return true if all setting was set
		*/
		
		bool setDefaultParams();
		
		
		/**
			Tries to conenct to GPRS
			Take some time
			
			@param apn ask your provider
			@return true if connected
		*/
		
		bool connectToGPRS(const char* apn);
		
		
		/**
			Tries to disconnect from GPRS
			Take some time
			
			@return true if connected
		*/
		
		bool disconnectFromGPRS();
		
		
		/**
			Initiates POST HTTP session and return handler of it
			
			@param address ip address
			@param url file path without first / character
			@param dataLength must match exactly amount you gonna write in
			
			@return post handler
		*/
		
		DataHandler<N>* sendPostRequest(IPAddress& address, const char* url, int dataLength);
		
		
		/**
			Initiates GET HTTP session and return handler of it
			
			@return get handler
		*/
		
		DataHandler<N>* sendGetRequest();
		
		void handleReading();
		
	private:
		void writeDefaultParam(int id);
		bool tryToSetDefaultParam(int id);
	
		FixedBuffer<N> buffer;
		SimIOWrapper<N> wrapper;
		UnexpectedHandler<N> reader;
		SimResultParser<N> parser;
		SimCommandPort simPort;
		TCPHandler<N> tcpHandler;
		GPRSHandler<N> gprsHandler;
		HTTPHandler<N> httpHandler;
		
};

template class SimHandler<FIXED_BUFFER_SIZE>;



