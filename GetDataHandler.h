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
		GetDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer);
		
		bool send() override;
		
};

template class GetDataHandler<128>;