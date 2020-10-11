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
		PostDataHandler(CommandWriter& wrapper, 
							SimResultParser<N>& parser, 
							SimCommandWriter& writer,
							BaseReader& reader,
							FixedBuffer<N>& buffer);

		bool send() override;
		
};

template class PostDataHandler<128>;