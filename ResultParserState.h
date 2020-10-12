#pragma once

#include "Buffer/FixedBuffer.h"


/**
	Will check both numberic and alpabetic codes
*/


template<int N>
class ResultParserStateBase
{
	public:
		virtual bool isSimpleMessageReady(FixedBuffer<N>& buffer);
		virtual bool isComplexMessageReady(FixedBuffer<N>& buffer);
		virtual bool isReadHttpMessageFull(FixedBuffer<N>& buffer);
		
		virtual int fetchResultCode(FixedBuffer<N>& buffer);
		
		virtual void removeReadHttpGarbage(FixedBuffer<N>& buffer);
		
	protected:
		int findLastIndexForRead(FixedBuffer<N>& buffer);
		virtual int getAmountToDeleteAfterRead();
		virtual bool isReadEnded(FixedBuffer<N>& buffer, int index);
		
};


template<int N>
class ResultParserStateText : public ResultParserStateBase<N>
{
	public:
		bool isSimpleMessageReady(FixedBuffer<N>& buffer) override;
		bool isComplexMessageReady(FixedBuffer<N>& buffer) override;
		
		int fetchResultCode(FixedBuffer<N>& buffer) override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(FixedBuffer<N>& buffer, int index) override;
};


template<int N>
class ResultParserStateDigit : public ResultParserStateBase<N>
{
	public:
		bool isSimpleMessageReady(FixedBuffer<N>& buffer) override;
		bool isComplexMessageReady(FixedBuffer<N>& buffer) override;
		
		int fetchResultCode(FixedBuffer<N>& buffer) override;
		
	protected:
		int getAmountToDeleteAfterRead() override;
		bool isReadEnded(FixedBuffer<N>& buffer, int index) override;
};



template class ResultParserStateBase<128>;
template class ResultParserStateText<128>;
template class ResultParserStateDigit<128>;