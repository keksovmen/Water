#pragma once

#include "CommandWriter.h"
#include "SimResultParser.h"
#include "SimCommandPort.h"
#include "Buffer/FixedBuffer.h"
#include "Constants.h"
#include "ResponceReader.h"


/**
	Handle base of http requests
	Childs must override send() to use their codes etc.
*/

template<int N>
class DataHandler : public BaseWriter, public ResponceReader<N>
{
	public:
		DataHandler(	SimResultParser<N>& parser, 
						SimCommandPort& simPort,
						FixedBuffer<N>& buffer
						);
		
		
		/**
			Implementation must send AT+HTTPACTION=<n>
			
			@return true if anwser after command was success
		*/
		
		virtual bool send() = 0;
		
		
		//delegates to refWriter
		void write(const char* str) override;
		void write(char c) override;
		void write(int i) override;
		void write(long l) override;
		void write(double d, int amountAfterDot) override;
		
		
		/**
			Check if request was sended fully on server
			
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
		
		// bool readResponce();
		
		// FixedBuffer<N>& getBuffer(){ return refBuffer;};
		
	protected:
		int getMinMessageLength() override;
		void removeGarbage() override;
		bool isMessageFull() override;
		void askForData(int index, int amount) override;
	
};

template class DataHandler<FIXED_BUFFER_SIZE>;