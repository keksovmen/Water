#pragma once

#include <IPAddress.h>
#include "CommandWriter.h"
#include "Enums.h"

/**
	Wrapper over SimIOWrapper for more easy command writing
	contain all command for sim module
	
	//TODO: made all comands as flesh memory at top of the file
*/


class SimCommandWriter
{
	public:
		explicit SimCommandWriter(CommandWriter& writer);
	
		void writeAT();
		void writeCREG();
		void writeCSQ();
		void writeEcho(bool turnOn);
		void writeNumberFormat(bool turnOn);
		void writeCallReady(bool turnOn);
		void writeReportAsError(bool turnOn);
		void writeSAPBR(SAPBR_COMMANDS cmd, const char* param = nullptr, const char* value = nullptr);
		void writeHTPP(HTTP_COMMANDS cmd);
		void writeHTTPURL(IPAddress& address, const char* url);
		void writeHTPPSetParam(const char* param, const char* value);
		void writeHTPPAction(HTTP_REQUESTS method);
		void writeHTPPData(int length);
		void writeReadHTTP(int from, int amount);
		void writeIPR(long rate);
		void writeCPIN();
		void writeDenyCall();
		void writeCIPRXGET(CIPRXGET_COMMAND cmd, int arg = 0);
		
		/**
			@param askForStatus true if whant to know current status
			@param turnOn true if won to try to attach to GPRS services
		*/
		
		void writeCGATT(bool askForStatus, bool turnOn = true);
		void writeCSTT(const char* apn);
		void writeCIICR();
		void writeCIPSTATUS();
		void writeCIPSTART(IPAddress& address, long port);
		void writeGetIpTCP();
		
	private:
		CommandWriter& refWriter;
		
};