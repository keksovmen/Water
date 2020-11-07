#pragma once

#include "ResponceReader.h"
#include "Constants.h"



class TCPReader : public ResponceReader
{
	public:
		TCPReader(	SimTools& tools,
					FixedBufferBase& buffer,
					unsigned long length
					);
		
	protected:
		int getMinMessageLength() override;
		void removeGarbage() override;
		bool isMessageFull() override;
		void askForData(int index, int amount) override;
		
		
	
};