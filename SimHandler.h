#pragma once

#include <Stream.h>
#include <IPAddress.h>
// #include "SimHandlerHelper.h"
#include "Enums.h"
#include "Constants.h"
#include "Buffer/FixedBufferBase.h"
#include "SimIOWrapper.h"
#include "UnexpectedHandler.h"
#include "SimTools.h"
#include "GPRSHandler.h"
#include "CgattHandler.h"
#include "HTTPHandler.h"
#include "TCPHandler.h"
#include "DataHandler.h"
#include "ParameterHandler.h"
#include "Activity.h"



/**
	BOSS OF THIS GYM
	
	Handles the highest lvl of interaction with sim

*/

template<int N>
class SimHandlerHelper;
// template class SimHandlerHelper<FIXED_BUFFER_SIZE>;
// template<> class SimHandlerHelper<128>;

class SimHandler : public Activity
{
	public:
		template<int N>
		friend class SimHandlerHelper;
	
		SimHandler(
				Stream& refPort,
				FixedBufferBase& buffer,
				ParameterHandler& parameters
				);
		
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
		
		DataHandler* sendPostRequest(IPAddress& address, const char* url, int dataLength);
		
		
		/**
			Initiates GET HTTP session and return handler of it
			
			@return get handler
		*/
		
		DataHandler* sendGetRequest(IPAddress& address, const char* url);
		
		void handleReading();
		
		void doActivity() override;
		
	private:
		void writeDefaultParam(int id);
		bool tryToSetDefaultParam(int id);
		void handleTCPMessage();
		bool handleLongMessages();
		bool isDefaultsAreSet();
		bool isNetworkEstablished();
		bool isCGATTAttached();
		bool isGPRSConnected();
		bool isTCPConnecting();
		bool isTCPWorking();
		

		FixedBufferBase& refBuffer;
		SimIOWrapper wrapper;
		UnexpectedHandler reader;
		SimToolsProxy tools;
		GPRSHandler gprsHandler;
		CgattHandler cgattHandler;
		HTTPHandler httpHandler;
		TCPHandler tcpHandler;
		
		ParameterHandler& refParams;
		
};



