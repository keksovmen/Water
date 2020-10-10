#pragma once

#include "SimIOWrapper.h"
#include "Enums.h"

/**
	Wrapper over SimIOWrapper for more easy command writing
	contain all command for sim module
	
	//TODO: made all comands as flesh memory at top of the file
*/


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
		void writeHTPPAction(HTTP_REQUESTS method);
		void writeHTPPData(int length);
		void writeReadHTTP(int from, int amount);
		void writeIPR(long rate);
		void writeCPIN();
		
	private:
		SimIOWrapper<N>& wrapper;
		
};

template class SimCommandWriter<128>;