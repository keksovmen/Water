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
	refWriter.writeSAPBR(SAPBR_COMMANDS::QUERY_BEARER);
	
	if(!readAndExpectSuccess(refWrapper, refParser, true)){
		return false;
	}

	BEARER_STATUS status = static_cast<BEARER_STATUS>(
		refParser.fetchGPRSStatus(refWrapper.getBuffer()));
		
	switch (status){
		case GPRS_CONNECTING:
		case GPRS_CONNECTED:
			return true;
		
		default: break;
	}
	
	return false;
}


/**

	@param apn must include double qouts on both sides
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

	if(!refWrapper.readToBufferTimeout(2000)){
		return false;
	}
	
	return isConnected();
}

template<int N>
bool GPRSHandler<N>::close(){
	refWriter.writeSAPBR(SAPBR_COMMANDS::CLOSE_BEARER);
	
	if(!refWrapper.readToBufferTimeout(2000)){
		return false;
	}
	
	return !isConnected();
}
