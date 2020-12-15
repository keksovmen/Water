#pragma once

#include "ResultParserState.h"
#include "Enums.h"
#include "Constants.h"


/**
	THE MAIN RULE:
	When you try to fetch some data,
	you MUST know it already there!
	Make sure through ready methods!
	
	Handles parsing of anwser from sim module
*/


class SimResultParser : public ResultParserStateBase
{
	public:
		explicit SimResultParser(FixedBufferBase& refBuffer);
		
		
		/**
			Changes state of some functions that depend on 
			text ot digit format
		
			@param state to expect data in that format
		*/
		
		void setState(PARSER_STATE state);
		
		
		/**
			Simple message has as an anwser only code
			
			Check if a simple message is arrived
			
			@return true if arrived
		*/
		
		bool isSimpleMessageReady() override;
		
		
		/**
			Check if message contain CME ERROR:
			
			@return true if there is such error
		*/
		
		bool checkError() override;
		
		
		/**
			Complex message has as an some responce and 
			anwser code
			
			Check if a complex message is arrived
			
			@return true if arrived
		*/
		
		bool isComplexMessageReady() override;
		
		
		/**
			@return true if AT+HTTPREAD or AT+CIPRXGET
				is present in buffer
		
		*/
		
		bool isReadMessageFull(READ_TYPE type) override;
		
		
		/**
			Expect anthing but has to end with <code>\r\n
		
			@return result code one of ANWSER_CODES
		*/
		
		int fetchResultCode() override;
		
		
		/**
			Modifies buffer removing from it
			garbare from AT+HTTPREAD and AT+CIPRXGET
		*/
		
		void removeReadGarbage(READ_TYPE type) override;
		
		
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
		
		
		/**
			Check variants at which message could presents
			in buffer
			
			@return true if there could be a message full or not undefinied
		*/
		
		bool isPossibleMessage();
		
		
		/**
			Check after AT+CGATT? call
			retrive status id and trunsfer it to bool
			
			@return true if status id is 1
		*/
		
		bool isAttachedToGPRSServices();
		
		
		/**
			Check and return status from AT+CIPSTATUS
			Params are:
				CIPMUX=0
				MODULE AS CLIENT
		*/
		
		TCP_STATE fetchTCPState();
		
		
		/**
			Gets status from AT+CIPRXGET?
			
			@return 0 if plain mode, 1 if command mode
		*/
		
		int fetchRxGetStatus();
		
		int parseRxGetLength();
		
		bool containShut();
		
	private:
		// ResultParserStateBase* pState;
		ResultParserStateBase* pState;
		
};