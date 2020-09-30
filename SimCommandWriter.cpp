#include "SimCommandWriter.h"


template<int N>
void SimCommandWriter<N>::writeAT(SimIOWrapper<N>& wrapper){
	wrapper.writeCommand("AT");
}

template<int N>
void SimCommandWriter<N>::writeCREG(SimIOWrapper<N>& wrapper){
	wrapper.writeCommand("AT+CREG?");
}

template<int N>
void SimCommandWriter<N>::writeCSQ(SimIOWrapper<N>& wrapper){
	wrapper.writeCommand("AT+CSQ");
}

template<int N>
void SimCommandWriter<N>::writeEcho(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		wrapper.writeCommand("ATE1&W");
	}else{
		wrapper.writeCommand("ATE0&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeNumberFormat(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		wrapper.writeCommand("ATV0&W");
	}else{
		wrapper.writeCommand("ATV1&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeCallReady(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		wrapper.writeCommand("AT+CIURC=1;&W");
	}else{
		wrapper.writeCommand("AT+CIURC=0;&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeReportAsError(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		wrapper.writeCommand("AT+CMEE=0;&W");
	}else{
		wrapper.writeCommand("AT+CMEE=1;&W");
	}
}


template<int N>
void SimCommandWriter<N>::writeSAPBR(SimIOWrapper<N>& wrapper, SAPBR_COMMANDS cmd, const char* param, const char* value){
	wrapper.writeString("AT+SAPBR=").
		writeInt(static_cast<int>(cmd)).
		writeString(",1");
		
	if((param) && (value)){
		wrapper.writeString(",\"").
			writeString(param).
			writeString("\",\"").
			writeString(value).
			writeChar('"');
	}
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeHTPP(SimIOWrapper<N>& wrapper, HTTP_COMMANDS cmd){
	switch (cmd){
		case HTTP_INIT:
			wrapper.writeCommand("AT+HTTPINIT");
		break;
		
		case HTTP_TERM:
			wrapper.writeCommand("AT+HTTPTERM");
		break;
			
		default: break;
	}
}


template<int N>
void SimCommandWriter<N>::writeHTPPSetParam(SimIOWrapper<N>& wrapper, const char* param, const char* value){
	wrapper.writeString("AT+HTTPPARA=").
		writeChar('"').
		writeString(param).
		writeString("\",\"").
		writeString(value).
		writeChar('"').
		writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeHTPPAction(SimIOWrapper<N>& wrapper, bool isPost){
	if(isPost){
		wrapper.writeCommand("AT+HTTPACTION=1");
	}else{
		wrapper.writeCommand("AT+HTTPACTION=0");
	}
}

template<int N>
void SimCommandWriter<N>::writeHTPPData(SimIOWrapper<N> wrapper, int length){
	wrapper.writeString("AT+HTTPDATA=").
		writeInt(length).
		writeChar(',').
		writeInt(5000).
		writeEndOfCommand();
}