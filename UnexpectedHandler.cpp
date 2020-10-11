#include "UnexpectedHandler.h"
#include "Enums.h"


template<int N>
UnexpectedHandler<N>::UnexpectedHandler(SimIOWrapper<N>& wrapper, 
							SimResultParser<N>& parser,
							SimCommandWriter& writer) :
	wrapper(wrapper), parser(parser), writer(writer)
{

}



template<int N>
bool UnexpectedHandler<N>::read(){
	auto& buffer = wrapper.getBuffer();
	
	if(!wrapper.read()){
		return false;
	}
	
	if(buffer.remove("\r\nRING\r\n")){
		//change unexpectedMessages
		handleIncomingCall();
	}
	
	if(buffer.remove("\r\nNO CARRIER\r\n")){
		//change unexpectedMessages
	}
	
	if(buffer.remove("\r\nUNDER-VOLTAGE WARNNING\r\n")){
		//change unexpectedMessages
	}
	
	if(buffer.remove("\r\nOVER-VOLTAGE WARNNING\r\n")){
		//change unexpectedMessages
	}
	
	return true;
}


template<int N>
bool readTimeout(unsigned long maxDelay){
	
}



template<int N>
void UnexpectedHandler<N>::handleIncomingCall(){
	writer.writeDenyCall();
	while(!readAndExpectSuccess(wrapper, parser)){};
}

