#pragma once

#include <SoftwareSerial.h>
#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"
#include "Enums.h"
#include "GPRSHandler.h"
#include "DataHandler.h"
#include "HTTPHandler.h"


/**
	BOSS OF THIS GYM
	
	Handles the highest lvl of interaction with sim

*/


template<int N>
class SimHandler
{
	public:
		SimHandler(SoftwareSerial& refPort);
		
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
			
			@param url where to send
			@param dataLength must match exactly amount you gonna write in
			
			@return post handler
		*/
		
		DataHandler<N>* sendPostRequest(const char* url, int dataLength);
		
		
		/**
			Initiates GET HTTP session and return handler of it
			
			@return get handler
		*/
		
		DataHandler<N>* sendGetRequest();
		
		void handleReading();
		
	private:
		SimIOWrapper<N> wrapper;
		SimCommandWriter writer;
		SimResultParser<N> parser;
		GPRSHandler<N> gprsHandler;
		HTTPHandler<N> httpHandler;
		
};

template class SimHandler<128>;



