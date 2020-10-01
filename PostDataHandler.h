#pragma once

#include "DataHandler.h"


template<int N>
class PostDataHandler : public DataHandler<N>
{
	public:
		PostDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer);

		bool send() override;
		
};

template class PostDataHandler<128>;
