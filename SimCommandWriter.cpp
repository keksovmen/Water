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
	wrapper.writeString("ATE");
	wrapper.writeChar(turnOn ? '1' : '0');
	wrapper.writeString("&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeNumberFormat(bool turnOn){
	wrapper.writeString("ATV");
	wrapper.writeChar(turnOn ? '0' : '1');
	wrapper.writeString("&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeCallReady(bool turnOn){
	wrapper.writeString("AT+CIURC=");
	wrapper.writeChar(turnOn ? '1' : '0');
	wrapper.writeString(";&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeReportAsError(bool turnOn){
	wrapper.writeString("AT+CMEE=");
	wrapper.writeChar(turnOn ? '1' : '0');
	wrapper.writeString(";&W");
	wrapper.writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeSAPBR(SAPBR_COMMANDS cmd, const char* param, const char* value){
	wrapper.writeString("AT+SAPBR=");
	wrapper.writeInt(static_cast<int>(cmd));
	wrapper.writeString(",1");
		
	if((param) && (value)){
		wrapper.writeString(",\"");
		wrapper.writeString(param);
		wrapper.writeString("\",\"");
		wrapper.writeString(value);
		wrapper.writeChar('"');
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
	wrapper.writeString("AT+HTTPPARA=");
	wrapper.writeChar('"');
	wrapper.writeString(param);
	wrapper.writeString("\",\"");
		
	if(value){
		wrapper.writeString(value);
		wrapper.writeChar('"');
		wrapper.writeEndOfCommand();
	}
	//else will be written later through other methods of SimIOWrapper
}


template<int N>
void SimCommandWriter<N>::writeHTPPAction(HTTP_REQUESTS method){
	wrapper.writeString("AT+HTTPACTION=");
	wrapper.writeInt(static_cast<int>(method));
	wrapper.writeEndOfCommand();
	
}

template<int N>
void SimCommandWriter<N>::writeHTPPData(int length){
	wrapper.writeString("AT+HTTPDATA=");
	wrapper.writeInt(length);
	wrapper.writeChar(',');
	wrapper.writeInt(5000);
	wrapper.writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeReadHTTP(int from, int amount){
	wrapper.writeString("AT+HTTPREAD=");
	wrapper.writeInt(from);
	wrapper.writeChar(',');
	wrapper.writeInt(amount);
	wrapper.writeEndOfCommand(false);
}