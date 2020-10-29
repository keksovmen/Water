#pragma once

#include "DataHandler.h"


/**
	Child of DataHandler
	Handles POST messages
	
	Write commands will write into POST body
*/

template<int N>
class PostDataHandler : public DataHandler<N>
{
	public:
		PostDataHandler(	SimResultParser<N>& parser, 
							SimCommandPort& simPort,
							FixedBuffer<N>& buffer,
							SimState& state
							);

		bool send() override;
		
};

template class PostDataHandler<128>;