#include "GPRSHandler.h"
#include "Enums.h"
#include "Util.h"


template<int N>
GPRSHandler<N>::GPRSHandler(SimIOWrapper<N>& wrapper, 
			SimCommandWriter<N>& writer,
			SimResultParser<N>& parser
			) : 
refWrapper(wrapper), refWriter(writer), refParser(parser){
	
}


template<int N>
bool GPRSHandler<N>::isConnected(){
	int result = (retriveStatus());
	
	if(result == -1){
		return false;
	}
	
	BEARER_STATUS status = static_cast<BEARER_STATUS>(result);
		
	switch (status){
		case GPRS_CONNECTING:
		case GPRS_CONNECTED:
			return true;
		
		default: break;
	}
	
	return false;
}


/**
	
	Max response time when open bearer is 85 sec
	
	@param apn must include double qouts on both sides
	@return true if bearer is opening or already open
*/


template<int N>
bool GPRSHandler<N>::connect(const char* apn){
	refWriter.writeSAPBR(SET_PARAM_BEARER,
						"Contype", "GPRS");
	
	if(!readAndExpectSuccess(refWrapper, refParser)){
		return false;
	}
	
	refWriter.writeSAPBR(SET_PARAM_BEARER,
						"APN", apn);
	
	if(!readAndExpectSuccess(refWrapper, refParser)){
		return false;
	}
	
	refWriter.writeSAPBR(OPEN_BEARER);

	if(!refWrapper.readToBufferTimeout(5000)){
		return false;
	}
	
	return isConnected();
}


/**
	Max response time when close bearer is 65 sec
	
	@return true is bearer is closing or already closed
*/

template<int N>
bool GPRSHandler<N>::close(){
	refWriter.writeSAPBR(SAPBR_COMMANDS::CLOSE_BEARER);
	
	if(!refWrapper.readToBufferTimeout(5000)){
		return false;
	}
	
	return !isConnected();
}


/**
	@return -1 if there is error of some sort
*/

template<int N>
int GPRSHandler<N>::retriveStatus(){
	refWriter.writeSAPBR(SAPBR_COMMANDS::QUERY_BEARER);
	
	if(!readAndExpectSuccess(refWrapper, refParser, true)){
		return -1;
	}
	
	return refParser.fetchGPRSStatus();
}