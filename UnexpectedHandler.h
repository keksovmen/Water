#pragma once

#include "BaseReader.h"
#include "SimIOWrapper.h"
#include "SimResultParser.h"
#include "SimCommandWriter.h"


/**
	Have to read data from SimIOWrapper,
	find something unexpected handle it,
	leave buffer without any trace of it
*/


template<int N>
class UnexpectedHandler : public BaseReader
{
	public:
		UnexpectedHandler(SimIOWrapper<N>& wrapper, 
							SimResultParser<N>& parser,
							SimCommandWriter& writer);
		
		
		
		
		/**
			@return same as SimIOWrapper
		*/
		
		bool read() override;
		
		bool readTimeout(unsigned long maxDelay) override;
		
		
		/**
			Represents if there was some unexpected messages
			such as: voltage, call, tcp incoming
			
			Digits as Power of 2, in binary form
		*/
		
		int unexpectedMessages = 0;
		
	private:
		void handleIncomingCall();
	
		SimIOWrapper<N>& wrapper;
		SimResultParser<N>& parser;
		SimCommandWriter& writer;
		
		
};

