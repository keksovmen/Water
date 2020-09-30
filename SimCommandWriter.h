#pragma once

#include "SimIOWrapper.h"
#include "Enums.h"

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
		bool writeSAPBR(SimIOWrapper<N>& wrapper, SAPBR_COMMANDS cmd, const char* param = nullptr, const char* value = nullptr);
		bool writeHTPP(SimIOWrapper<N>& wrapper, HTTP_COMMANDS cmd);
		bool writeHTPPSetParam(SimIOWrapper<N>& wrapper, const char* param, const char* value);
		bool writeHTPPAction(SimIOWrapper<N>& wrapper, bool isPost);
		bool writeHTPPData(SimIOWrapper<N> wrapper, int length);
	private:
		//TODO: made SimIOWrapper as refParam
		
};

template class SimCommandWriter<128>;