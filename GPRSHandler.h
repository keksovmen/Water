#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"


/**
	Handles GPRS connection disconnection

*/

template<int N>
class GPRSHandler
{
	public:
		GPRSHandler(SimIOWrapper<N>& wrapper, 
					SimCommandWriter<N>& writer,
					SimResultParser<N>& parser
					);
		
		
		/**
			Check if module is connected to network
			
			//TODO: made instead of bool use enum,
			//there is CONNECTING status that can fail
			@return network connection status
		*/	
		
		bool isConnected();
		
		
		/**
			Tries to conenct to GPRS
			
			@pram apn ask provider for it
			@return true if successfully connected
		*/
		
		bool connect(const char* apn);
		
		
		/**
			Tries to close conenction
			
			@return true if closed
		*/
		
		bool close();
		
		
	private:
	
		//short cut for connect() method
		int retriveStatus();
	
		SimIOWrapper<N>& refWrapper;
		SimCommandWriter<N>& refWriter;
		SimResultParser<N>& refParser;
		
		
};

template class GPRSHandler<128>;