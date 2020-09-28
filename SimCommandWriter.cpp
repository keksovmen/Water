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

