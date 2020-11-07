#pragma once

#include "CommandWriter.h"
#include "Constants.h"
#include "ResponceReader.h"
#include "LongCommandHandler.h"


/**
	Handle base of http requests
	Childs must override send() to use their codes etc.
*/

class DataHandler : public BaseWriter, public ResponceReader, public LongCommandHandler
{
	public:
		DataHandler(	SimTools& tools,
						FixedBufferBase& buffer
						);
		
		
		/**
			Implementation must send AT+HTTPACTION=<n>
			
			@return true if anwser after command was success
		*/
		
		virtual bool send();
		
		
		//delegates to refWriter
		void write(const char* str) override;
		void write(char c) override;
		void write(int i) override;
		void write(long l) override;
		void write(double d, int amountAfterDot) override;
		
		bool handle() override;
		
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
		
	protected:
		int getMinMessageLength() override;
		void removeGarbage() override;
		bool isMessageFull() override;
		void askForData(int index, int amount) override;
		
	
};