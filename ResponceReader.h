#pragma once

#include "SimResultParser.h"
#include "SimCommandPort.h"
#include "Buffer/FixedBufferBase.h"
#include "Constants.h"


class ResponceReader
{
	public:
		ResponceReader(	SimResultParser& parser, 
						SimCommandPort& simPort,
						FixedBufferBase& buffer
						);
						
		bool readResponce();
		
		FixedBufferBase& getBuffer(){ return refBuffer;};
		
	protected:
		virtual int getMinMessageLength() = 0;
		virtual void removeGarbage() = 0;
		virtual bool isMessageFull() = 0;
		virtual void askForData(int index, int amount) = 0;
		
	
		unsigned int readIndex = 0;
		unsigned long responceLength = 0;
		//meybe not neccessery
		bool firstRead = true;
		
		SimResultParser& refParser;
		SimCommandPort& refPort;
		FixedBufferBase& refBuffer;
		
	
};