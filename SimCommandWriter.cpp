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
	wrapper.write("ATE");
	wrapper.write(turnOn ? '1' : '0');
	wrapper.write("&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeNumberFormat(bool turnOn){
	wrapper.write("ATV");
	wrapper.write(turnOn ? '0' : '1');
	wrapper.write("&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeCallReady(bool turnOn){
	wrapper.write("AT+CIURC=");
	wrapper.write(turnOn ? '1' : '0');
	wrapper.write(";&W");
	wrapper.writeEndOfCommand();
}

template<int N>
void SimCommandWriter<N>::writeReportAsError(bool turnOn){
	wrapper.write("AT+CMEE=");
	wrapper.write(turnOn ? '1' : '0');
	wrapper.write(";&W");
	wrapper.writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeSAPBR(SAPBR_COMMANDS cmd, const char* param, const char* value){
	wrapper.write("AT+SAPBR=");
	wrapper.write(static_cast<int>(cmd));
	wrapper.write(",1");
		
	if((param) && (value)){
		wrapper.write(",\"");
		wrapper.write(param);
		wrapper.write("\",\"");
		wrapper.write(value);
		wrapper.write('"');
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
	wrapper.write("AT+HTTPPARA=");
	wrapper.write('"');
	wrapper.write(param);
	wrapper.write("\",\"");
		
	if(value){
		wrapper.write(value);
		wrapper.write('"');
		wrapper.writeEndOfCommand();
	}
	//else will be written later through other methods of SimIOWrapper
}


template<int N>
void SimCommandWriter<N>::writeHTPPAction(HTTP_REQUESTS method){
	wrapper.write("AT+HTTPACTION=");
	wrapper.write(static_cast<int>(method));
	wrapper.writeEndOfCommand();
	
}

template<int N>
void SimCommandWriter<N>::writeHTPPData(int length){
	wrapper.write("AT+HTTPDATA=");
	wrapper.write(length);
	wrapper.write(',');
	wrapper.write(5000);
	wrapper.writeEndOfCommand();
}


template<int N>
void SimCommandWriter<N>::writeReadHTTP(int from, int amount){
	wrapper.write("AT+HTTPREAD=");
	wrapper.write(from);
	wrapper.write(',');
	wrapper.write(amount);
	wrapper.writeEndOfCommand(false);
}


template<int N>
void SimCommandWriter<N>::writeIPR(long rate){
	wrapper.write("AT+IPR=");
	wrapper.write(rate);
	wrapper.write(";&W");
	wrapper.writeEndOfCommand();
}