#pragma once

#include "DataHandler.h"


/**
	Child of DataHandler
	Handles POST messages
	
	Write commands will write into URL body
*/

template<int N>
class GetDataHandler : public DataHandler<N>
{
	public:
		GetDataHandler(	SimResultParser<N>& parser, 
						SimCommandPort& simPort,
						FixedBuffer<N>& buffer
						);
		
		bool send() override;
		
};

template class GetDataHandler<128>;