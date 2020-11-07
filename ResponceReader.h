#pragma once


#include "Constants.h"
#include "SimTools.h"
#include "Buffer/FixedBufferBase.h"


class ResponceReader
{
	public:
		ResponceReader(	SimTools& tools,
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
		
		SimTools& refTools;
		FixedBufferBase& refBuffer;
		
	
};