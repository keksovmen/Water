#pragma once

#include "ResponceReader.h"
#include "Constants.h"



class TCPReader : public ResponceReader
{
	public:
		TCPReader(	SimResultParser& parser, 
					SimCommandPort& simPort,
					FixedBufferBase& buffer,
					unsigned long length
					);
		
	protected:
		int getMinMessageLength() override;
		void removeGarbage() override;
		bool isMessageFull() override;
		void askForData(int index, int amount) override;
		
		
	
};