#pragma once

#include "SimResultParser.h"
#include "SimCommandPort.h"
#include "Buffer/FixedBuffer.h"
#include "Constants.h"

template<int N>
class ResponceReader
{
	public:
		ResponceReader(	SimResultParser<N>& parser, 
						SimCommandPort& simPort,
						FixedBuffer<N>& buffer
						);
						
		bool readResponce();
		
		FixedBuffer<N>& getBuffer(){ return refBuffer;};
		
	protected:
		virtual int getMinMessageLength() = 0;
		virtual void removeGarbage() = 0;
		virtual bool isMessageFull() = 0;
		virtual void askForData(int index, int amount) = 0;
		
	
		unsigned int readIndex = 0;
		unsigned long responceLength = 0;
		//meybe not neccessery
		bool firstRead = true;
		
		SimResultParser<N>& refParser;
		SimCommandPort& refPort;
		FixedBuffer<N>& refBuffer;
		
	
};

template class ResponceReader<FIXED_BUFFER_SIZE>;