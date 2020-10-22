#pragma once

#include "Buffer/FixedBuffer.h"
#include "Constants.h"


/**
	Will check both numberic and alpabetic codes
*/


template<int N>
class ResultParserStateBase
{
	public:
		explicit ResultParserStateBase(FixedBuffer<N>& refBuffer) : refBuffer(refBuffer){}
	
		virtual bool isSimpleMessageReady();
		virtual bool isComplexMessageReady();
		virtual bool isReadHttpMessageFull();
		virtual bool checkError();
		
		virtual int fetchResultCode();
		
		virtual void removeReadHttpGarbage();
		
		
		/**
			@return error code from CME ERROR: <n>
		*/
		
		int getLastError(){return lastErrorCode;}
		
	protected:
		int findLastIndexForRead();
		virtual int getAmountToDeleteAfterRead();
		virtual bool isReadEnded(int index);
		
		
		//Indicate last error,
		//-1 is no error 
		int lastErrorCode = -1;
		
		FixedBuffer<N>& refBuffer;
		
};


template<int N>
class ResultParserStateText : public ResultParserStateBase<N>
{
	public:
		explicit ResultParserStateText(FixedBuffer<N>& refBuffer) : ResultParserStateBase<N>(refBuffer){}
		
		bool isSimpleMessageReady() override;
		bool isComplexMessageReady() override;
		
		int fetchResultCode() override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(int index) override;
};


//NOT TESTED WITH HTTPREAD

template<int N>
class ResultParserStateDigit : public ResultParserStateBase<N>
{
	public:
		explicit ResultParserStateDigit(FixedBuffer<N>& refBuffer) : ResultParserStateBase<N>(refBuffer){}
	
		bool isSimpleMessageReady() override;
		bool isComplexMessageReady() override;
		
		int fetchResultCode() override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(int index) override;
};



template class ResultParserStateBase<FIXED_BUFFER_SIZE>;
template class ResultParserStateText<FIXED_BUFFER_SIZE>;
template class ResultParserStateDigit<FIXED_BUFFER_SIZE>;