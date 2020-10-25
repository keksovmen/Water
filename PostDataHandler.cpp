#include "PostDataHandler.h"
#include "Util.h"


template <int N>
PostDataHandler<N>::PostDataHandler(	SimResultParser<N>& parser, 
										SimCommandPort& simPort,
										FixedBuffer<N>& buffer
										) :
	DataHandler<N>(parser, simPort, buffer){
		
}


template<int N>
bool PostDataHandler<N>::send(){
	this->refPort.writeEndOfCommand();
	
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	return readAndExpectSuccess(this->refPort, this->refParser);
}
