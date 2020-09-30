#pragma once

#include "SimIOWrapper.h"
#include "Enums.h"

template<int N>
class SimCommandWriter
{
	public:
		void writeAT(SimIOWrapper<N>& wrapper);
		void writeCREG(SimIOWrapper<N>& wrapper);
		void writeCSQ(SimIOWrapper<N>& wrapper);
		void writeEcho(SimIOWrapper<N>& wrapper, bool turnOn);
		void writeNumberFormat(SimIOWrapper<N>& wrapper, bool turnOn);
		void writeCallReady(SimIOWrapper<N>& wrapper, bool turnOn);
		void writeReportAsError(SimIOWrapper<N>& wrapper, bool turnOn);
		void writeSAPBR(SimIOWrapper<N>& wrapper, SAPBR_COMMANDS cmd, const char* param = nullptr, const char* value = nullptr);
		void writeHTPP(SimIOWrapper<N>& wrapper, HTTP_COMMANDS cmd);
		void writeHTPPSetParam(SimIOWrapper<N>& wrapper, const char* param, const char* value);
		void writeHTPPAction(SimIOWrapper<N>& wrapper, bool isPost);
		void writeHTPPData(SimIOWrapper<N> wrapper, int length);
	private:
		//TODO: made SimIOWrapper as refParam
		
};

template class SimCommandWriter<128>;