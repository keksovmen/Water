#pragma once

#include "SimIOWrapper.h"

template<int N>
class SimCommandWriter
{
	public:
		bool writeAT(SimIOWrapper<N>& wrapper);
		bool writeCREG(SimIOWrapper<N>& wrapper);
		bool writeCSQ(SimIOWrapper<N>& wrapper);
		
	private:
		
		
};

template class SimCommandWriter<128>;