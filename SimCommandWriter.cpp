#include "SimCommandWriter.h"


template<int N>
bool SimCommandWriter<N>::writeAT(SimIOWrapper<N>& wrapper){
	return wrapper.writeCommand("AT");
}

template<int N>
bool SimCommandWriter<N>::writeCREG(SimIOWrapper<N>& wrapper){
	return wrapper.writeCommand("AT+CREG?");
}

template<int N>
bool SimCommandWriter<N>::writeCSQ(SimIOWrapper<N>& wrapper){
	return wrapper.writeCommand("AT+CSQ");
}

template<int N>
bool SimCommandWriter<N>::writeEcho(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		return wrapper.writeCommand("ATE1&W");
	}else{
		return wrapper.writeCommand("ATE0&W");
	}
}

template<int N>
bool SimCommandWriter<N>::writeNumberFormat(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		return wrapper.writeCommand("ATV0&W");
	}else{
		return wrapper.writeCommand("ATV1&W");
	}
}

template<int N>
bool SimCommandWriter<N>::writeCallReady(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		return wrapper.writeCommand("AT+CIURC=1;&W");
	}else{
		return wrapper.writeCommand("AT+CIURC=0;&W");
	}
}

template<int N>
bool SimCommandWriter<N>::writeReportAsError(SimIOWrapper<N>& wrapper, bool turnOn){
	if(turnOn){
		return wrapper.writeCommand("AT+CMEE=0;&W");
	}else{
		return wrapper.writeCommand("AT+CMEE=1;&W");
	}
}


template<int N>
bool SimCommandWriter<N>::writeSAPBR(SimIOWrapper<N>& wrapper, SAPBR_COMMANDS cmd, const char* param, const char* value){
	wrapper.writeString("AT+SAPBR=").
		writeInt(static_cast<int>(cmd)).
		writeString(",1");
	if((param) && (value)){
		
		wrapper.writeChar(',').
			writeString(param).
			writeChar(',').
			writeString(value);
	}
	wrapper.writeSendCommand();
	return true;
}
