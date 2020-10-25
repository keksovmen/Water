#pragma once

#include "ResponceReader.h"
#include "Constants.h"



template<int N>
class TCPReader : public ResponceReader<N>
{
	public:
		TCPReader(	SimResultParser<N>& parser, 
					SimCommandPort& simPort,
					FixedBuffer<N>& buffer,
					unsigned long length
					);
		
	protected:
		int getMinMessageLength() override;
		void removeGarbage() override;
		bool isMessageFull() override;
		void askForData(int index, int amount) override;
		
		
	
};


template class TCPReader<FIXED_BUFFER_SIZE>;