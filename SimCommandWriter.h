#pragma once

#include "SimIOWrapper.h"
#include "Enums.h"

template<int N>
class SimCommandWriter
{
	public:
		explicit SimCommandWriter(SimIOWrapper<N>& refWrapper);
	
		void writeAT();
		void writeCREG();
		void writeCSQ();
		void writeEcho(bool turnOn);
		void writeNumberFormat(bool turnOn);
		void writeCallReady(bool turnOn);
		void writeReportAsError(bool turnOn);
		void writeSAPBR(SAPBR_COMMANDS cmd, const char* param = nullptr, const char* value = nullptr);
		void writeHTPP(HTTP_COMMANDS cmd);
		void writeHTPPSetParam(const char* param, const char* value);
		void writeHTPPAction(bool isPost);
		void writeHTPPData(int length);
		
	private:
		SimIOWrapper<N>& wrapper;
		
};

template class SimCommandWriter<128>;