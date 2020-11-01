#pragma once
#include <Stream.h>
#include "SimHandler.h"
#include "ParameterHandler.h"
#include "Parameter.h"
#include "Constants.h"
#include "TCPHandler.h"



template<int N>
class SimHandlerHelper
{
	public:
		SimHandlerHelper(Stream& connection, ParameterHandler& parameters);
		
		bool init();
		bool sendParams(ParameterHandler& params);
		bool sendVolume(Parameter<PrimitivIntParameter<int>>& volume, ParameterHandler& params);
		bool askTime();
		
		
		SimHandler<N>& getHandler(){return handler;}
		
	private:
		
		/**
			Call before each method to make sure that sim module
			is propely initialized before using
			
			@return state of isInit true if ready to use
		*/
		
		bool initRotine();
		
		
		/**
			Call before sending data to server
			
			@return true if module is up and network connection is set
		*/
		
		bool checkReadyToSend();
		
		
		/**
			Will decide what to do throug switch
			May call delay while network is serching
			
			@return true if network registered
		*/
		
		bool handleNetworkStatus(NETWORK_CONNECTION status);
	
		
		/**
			Handle basic rotine with sending and waiting for responce
			
			@return true only if successfuly sended data to the server
		*/
		
		bool handleSendRootine(DataHandler* dataHandler);
	
	
		SimHandler<N> handler;
		// TCPHandler<N> tcpHandler;
		ParameterHandler& refParameters;
		
		bool isInit = false;
		
		
	
};


template class SimHandlerHelper<FIXED_BUFFER_SIZE>;