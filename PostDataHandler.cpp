#include "PostDataHandler.h"
#include "Util.h"


template <int N>
PostDataHandler<N>::PostDataHandler(	SimResultParser<N>& parser, 
										SimCommandPort& simPort,
										FixedBuffer<N>& buffer,
										SimState& state
										) :
	DataHandler<N>(parser, simPort, buffer, state){
		
}


template<int N>
bool PostDataHandler<N>::send(){
	this->refPort.writeEndOfCommand();
	
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	bool result = readAndExpectSuccess(this->refPort, this->refParser);
	if(result){
		DataHandler<N>::send();
	}
	
	return result;
}
