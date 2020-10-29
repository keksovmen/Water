#include "GetDataHandler.h"
#include "Util.h"


template <int N>
GetDataHandler<N>::GetDataHandler(	SimResultParser<N>& parser, 
									SimCommandPort& simPort,
									FixedBuffer<N>& buffer,
									SimState& state
									) :
	DataHandler<N>(parser, simPort, buffer, state){
		
}


template<int N>
bool GetDataHandler<N>::send(){
	this->refPort.write('"');
	this->refPort.writeEndOfCommand();
			
	if(!readAndExpectSuccess(this->refPort, this->refParser)){
		return false;
	}
		
	this->refPort.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	bool result = readAndExpectSuccess(this->refPort, this->refParser);
	if(result){
		DataHandler<N>::send();
	}
	
	return result;
}