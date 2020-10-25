#include "GetDataHandler.h"
#include "Util.h"


template <int N>
GetDataHandler<N>::GetDataHandler(	SimResultParser<N>& parser, 
									SimCommandPort& simPort,
									FixedBuffer<N>& buffer
									) :
	DataHandler<N>(parser, simPort, buffer){
		
}


template<int N>
bool GetDataHandler<N>::send(){
	this->refPort.write('"');
	this->refPort.writeEndOfCommand();
			
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	return readAndExpectSuccess(this->refPort, this->refParser);
}