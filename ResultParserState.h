#pragma once

#include "Buffer/FixedBufferBase.h"
#include "Constants.h"
#include "Enums.h"


/**
	Will check both numberic and alpabetic codes
*/


class ResultParserStateBase
{
	public:
		explicit ResultParserStateBase(FixedBufferBase& refBuffer)
				: refBuffer(refBuffer){}
	
		virtual bool isSimpleMessageReady();
		virtual bool isComplexMessageReady();
		
		/**
			Removes onlu positive result
			
			@return true if removed
		*/
		
		virtual bool removeResultCode();
		
		
		/**
			Works for AT+HTTPREAD and AT+CIPRXGET
			
			@param str one of HTTPREAD_ANWSER, TCP_READ_ANWSER
		*/
		
		virtual bool isReadMessageFull(READ_TYPE type);
		virtual bool checkError();
		
		virtual int fetchResultCode();
		
		
		/**
			Works for AT+HTTPREAD and AT+CIPRXGET
			
			@param type what to expect
		*/
		
		virtual void removeReadGarbage(READ_TYPE type);
		
		
		/**
			@return error code from CME ERROR: <n>
		*/
		
		int getLastError(){return lastErrorCode;}
		
	protected:
		int findLastIndexForRead(const char* str);
		const char* routeReadType(READ_TYPE type);
		virtual int getAmountToDeleteAfterRead();
		virtual bool isReadEnded(int index);
		
		
		//Indicate last error,
		//-1 is no error 
		int lastErrorCode = -1;
		
		FixedBufferBase& refBuffer;
		
};


class ResultParserStateText : public ResultParserStateBase
{
	public:
		explicit ResultParserStateText(FixedBufferBase& refBuffer)
				: ResultParserStateBase(refBuffer){}
		
		bool isSimpleMessageReady() override;
		bool isComplexMessageReady() override;
		virtual bool removeResultCode() override;

		
		int fetchResultCode() override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(int index) override;
};


//NOT TESTED WITH HTTPREAD

class ResultParserStateDigit : public ResultParserStateBase
{
	public:
		explicit ResultParserStateDigit(FixedBufferBase& refBuffer)
				: ResultParserStateBase(refBuffer){}
	
		bool isSimpleMessageReady() override;
		bool isComplexMessageReady() override;
		virtual bool removeResultCode() override;
		
		int fetchResultCode() override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(int index) override;
};