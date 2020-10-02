#include "SimCommandWriter.h"


template<int N>
SimCommandWriter<N>::SimCommandWriter(SimIOWrapper<N>& refWrapper) :
	wrapper (refWrapper){
	
}

template<int N>
void SimCommandWriter<N>::writeAT(){
	wrapper.writeCommand("AT");
}

template<int N>
void SimCommandWriter<N>::writeCREG(){
	wrapper.writeCommand("AT+CREG?");
}

template<int N>
void SimCommandWriter<N>::writeCSQ(){
	wrapper.writeCommand("AT+CSQ");
}

template<int N>
void SimCommandWriter<N>::writeEcho(bool turnOn){
	if(turnOn){
		wrapper.writeCommand("ATE1&W");
	}else{
		wrapper.writeCommand("ATE0&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeNumberFormat(bool turnOn){
	if(turnOn){
		wrapper.writeCommand("ATV0&W");
	}else{
		wrapper.writeCommand("ATV1&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeCallReady(bool turnOn){
	if(turnOn){
		wrapper.writeCommand("AT+CIURC=1;&W");
	}else{
		wrapper.writeCommand("AT+CIURC=0;&W");
	}
}

template<int N>
void SimCommandWriter<N>::writeReportAsError(bool turnOn){
	if(turnOn){
		wrapper.writeCommand("AT+CMEE=0;&W");
	}else{
		wrapper.writeCommand("AT+CMEE=1;&W");
	}
}


template<int N>
void SimCommandWriter<N>::writeSAPBR(SAPBR_COMMANDS cmd, const char* param, const char* value){
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
void SimCommandWriter<N>::writeHTPP(HTTP_COMMANDS cmd){
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
void SimCommandWriter<N>::writeHTPPSetParam(const char* param, const char* value){
	//TODO: made separate header file with all comands as fields
	wrapper.writeString("AT+HTTPPARA=").
		writeChar('"').
		writeString(param).
		writeString("\",\"");
		
	if(value){
		wrapper.writeString(value).
		writeChar('"').
		writeEndOfCommand();
	}
	//else will be written later through other methods of SimIOWrapper
}


template<int N>
void SimCommandWriter<N>::writeHTPPAction(HTTP_REQUESTS method){
	wrapper.writeString("AT+HTTPACTION=").
		writeInt(static_cast<int>(method)).
		writeEndOfCommand();
	
}

template<int N>
void SimCommandWriter<N>::writeHTPPData(int length){
	wrapper.writeString("AT+HTTPDATA=").
		writeInt(length).
		writeChar(',').
		writeInt(5000).
		writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeReadHTTP(int from, int amount){
	wrapper.writeString("AT+HTTPREAD=").
		writeInt(from).
		writeChar(',').
		writeInt(amount).
		writeEndOfCommand(false);
}