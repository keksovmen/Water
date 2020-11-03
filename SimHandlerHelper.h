#pragma once

#include <Stream.h>
#include "Buffer/FixedBuffer.h"
#include "SimHandler.h"
#include "ParameterHandler.h"
#include "Parameter.h"
#include "Constants.h"
#include "TCPHandler.h"

/**
	How to use:
		If you want to send http:
			1	-	call isAbleToUseHttp() to check if it possible
			2	-	call sendSomething()
			3	-	call is anwserRdy()
			4	-	call isAnwserSuccess()
		If need result:
			4	-	call getResult()
		
		If need to abort:
			5	-	call abort()
			
*/

template<int N>
class SimHandlerHelper
{
	public:
		// friend class SimHandler;
		
		SimHandlerHelper(Stream& connection, ParameterHandler& parameters);
		
		bool sendParams(ParameterHandler& params);
		bool sendVolume(Parameter<PrimitivIntParameter<int>>& volume, ParameterHandler& params);
		bool askTime();
		bool isAbleToUseHttp();
		bool isAnwserRdy();
		bool isAnwserSuccess();
		
		
		SimHandler& getHandler(){return handler;}
		
	private:
		
		
		void finishSession();
		/**
			Call before each method to make sure that sim module
			is propely initialized before using
			
			@return state of isInit true if ready to use
		*/
		
		// bool initRotine();
		
		
		/**
			Call before sending data to server
			
			@return true if module is up and network connection is set
		*/
		
		// bool checkReadyToSend();
		
		
		/**
			Will decide what to do throug switch
			May call delay while network is serching
			
			@return true if network registered
		*/
		
		// bool handleNetworkStatus(NETWORK_CONNECTION status);
	
		
		/**
			Handle basic rotine with sending and waiting for responce
			
			@return true only if successfuly sended data to the server
		*/
		
		bool handleSendRootine();
	
		FixedBuffer<N> buffer;
		SimHandler handler;
		ParameterHandler& refParameters;
		
		DataHandler* dataHandler = nullptr;
		HTTP_SCRIPT lastRequest = HTTP_SCRIPT_NAN;
		
		bool isInit = false;
		
		
	
};


template class SimHandlerHelper<FIXED_BUFFER_SIZE>;