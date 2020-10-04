#pragma once

#include <SoftwareSerial.h>
#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"
#include "Enums.h"
#include "GPRSHandler.h"
#include "PostDataHandler.h"
#include "GetDataHandler.h"


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
			
			//TODO: made return pointer to DataHandler as base class
					or nullptr if there was an error
			@return post handler
		*/
		
		PostDataHandler<N> sendPostRequest(const char* url, int dataLength);
		
		
		/**
			Initiates GET HTTP session and return handler of it
			
			//TODO: made return pointer to DataHandler as base class
					or nullptr if there was an error
			@return get handler
		*/
		
		GetDataHandler<N> sendGetRequest();
		
	private:
		SimIOWrapper<N> wrapper;
		SimCommandWriter<N> writer;
		SimResultParser<N> parser;
		GPRSHandler<N> gprsHandler;
		
};

template class SimHandler<128>;



