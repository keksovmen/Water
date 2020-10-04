#pragma once

#include "SimIOWrapper.h"
#include "SimCommandWriter.h"
#include "SimResultParser.h"


/**
	Handle base of http requests
	Childs must override send() to use their codes etc.
*/

template<int N>
class DataHandler
{
	public:
		DataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer);
		
		
		/**
			Implementation must send AT+HTTPACTION=<n>
			
			@return true if anwser after command was success
		*/
		
		virtual bool send() = 0;
		
		
		void writeString(const char* str);
		void writeChar(char c);
		void writeInt(int val);
		
		
		/**
			Check if request was sended fully on server
			Currently consume time just for waiting!
			//TODO: remove time consumtion and make as simple check
			
			@return true if send completed
		*/
		
		bool isSended();
		
		
		/**
			Check result http responce status
			
			@return if statsu from 2xx family
		*/
		
		bool isSendedSuccesfully();
		
		
		/**
			Terminate HTTPSESSION
		*/
		
		void finish();
		
		
		/**
			Tries read data from server
			
			//TODO: made return status of buffer read, full or no more data
			@return true if at least 1 character was read
					or buffer full;
		*/
		
		bool readResponce();
		
		FixedBuffer<N>& getBuffer(){ return refWrapper.getBuffer();};
		
	protected:
		SimIOWrapper<N>& refWrapper;
		SimResultParser<N>& refParser;
		SimCommandWriter<N>& refWriter;
		
		unsigned int readIndex = 0;
		
		unsigned long responceLength = 0;
		
	
};

template class DataHandler<128>;