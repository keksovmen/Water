#pragma once
#include "Buffer/FixedBuffer.h"


/**
	THE MAIN RULE:
	When you try to fetch some data,
	you MUST know it already there!
	Make sure through ready methods!
	
	Handles parsing of anwser from sim module
*/


template<int N>
class SimResultParser
{
	public:
		explicit SimResultParser(FixedBuffer<N>& refBuffer);
		
		
		/**
			Simple message has as an anwser only code
			
			Check if a simple message is arrived
			
			@return true if arrived
		*/
		
		bool isSimpleMessageReady();
		
		
		/**
			Complex message has as an some responce and 
			anwser code
			
			Check if a complex message is arrived
			
			@return true if arrived
		*/
		
		bool isComplexMessageReady();
		
		
		/**
			@return true if buffer has DONWLOAD string
		*/
		
		bool containDownload();
		
		
		/**
			@return true if buffer has 
				+HTTPACTION: <method>,<statuc>,<dataLength>\r\n
		*/
		
		bool isHttpActionPresents();
		
		
		/**
			@return true if AT+HTTPREAD presents as whole string
				+HTTPREAD: <length>\r\nDATA...<code>\r\n
		
		*/
		
		bool isReadHttpMessageFull();
		
		
		/**
			Expect anthing but has to end with <code>\r\n
		
			@return result code one of ANWSER_CODES
		*/
		
		int fetchResultCode();
		
		
		/**
			Expect buffer to contain message:
			+CREG: <n>,<status>\r\n
			
			@return <status> one of NETWORK_CONNECTION
		*/
		
		int fetchNetworkRegistration();
		
		
		/**
			Expect buffer to contain message:
			+SAPBR: <cid>,<status>\r\n
			
			@return <status> one of BEARER_STATUS
		*/
		
		int fetchGPRSStatus();
		
		
		/**
			Expect buffer to contain message:
			+HTTPACTION: <method>,<status>,<data length>
			
			@return <status> one of HTTP_STATUS_CODES
		*/
		
		int fetchHTTPStatus();
		
		
		/**
			Expect buffer to contain message:
			+HTTPACTION: <method>,<status>,<data length>
			
			@return <data length>
		*/
		
		unsigned long fetchHttpResponceLength();
		
		
		/**
			Modifies buffer removing from it
			garbare such as \r\nHTTPREAD: <length>\r\n 
				and status code at the end
		*/
		
		void removeReadHttpGarbage();
		
		
		
		/**
			Expects result from AT+CPIN?
			+CPIN: <code>
				code - READY
						SIM PIN
						SIM PUK
						PH_SIM PIN
						PH_SIM PUK
						SIM PIN2
		*/
		
		bool isPinRdy();
		
	private:
		FixedBuffer<N>& buffer;
		
};


template class SimResultParser<128>;
