#pragma once

#include "Buffer/FixedBufferBase.h"
#include "BaseReader.h"
#include "UnexpectedHandler.h"
#include "Constants.h"
#include "SimState.h"


/**
	Have to read data from SimIOWrapper,
	find something unexpected handle it,
	leave buffer without any trace of it
*/


class ReaderWithoutGarbage : public BaseReader, public UnexpectedHandler
{
	public:
		ReaderWithoutGarbage(	FixedBufferBase& buffer,
							BaseReader& reader,
							SimState& state
							);
		
		
		//delegate to refReader
		bool read() override;
		bool readTimeout(unsigned long maxDelay) override;
		bool lazyRead() override;
		
		void handleUnexpected(FixedBufferBase& refBuffer) override;
		
		
	private:
		void handleIncomingCall();
		
		bool readRemovingGarbage();
	
		FixedBufferBase& refBuffer;
		BaseReader& refReader;
		SimState& refState;
};