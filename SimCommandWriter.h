#pragma once

#include "SimIOWrapper.h"

template<int N>
class SimCommandWriter
{
	public:
		bool writeAT(SimIOWrapper<N>& wrapper);
		bool writeCREG(SimIOWrapper<N>& wrapper);
		bool writeCSQ(SimIOWrapper<N>& wrapper);
		bool writeEcho(SimIOWrapper<N>& wrapper, bool turnOn);
		bool writeNumberFormat(SimIOWrapper<N>& wrapper, bool turnOn);
		bool writeCallReady(SimIOWrapper<N>& wrapper, bool turnOn);
		bool writeReportAsError(SimIOWrapper<N>& wrapper, bool turnOn);
		
		
	private:
		
		
};

template class SimCommandWriter<128>;