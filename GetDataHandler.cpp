#include "GetDataHandler.h"
#include "Util.h"

template <int N>
GetDataHandler<N>::GetDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer) :
	DataHandler<N>(wrapper, parser, writer){
		
}


template<int N>
bool GetDataHandler<N>::send(){
	this->refWrapper.writeChar('"').
			writeEndOfCommand();
			
	if(!readAndExpectSuccess(this->refWrapper, this->refParser)){
		return false;
	}
		
	this->refWriter.writeHTPPAction(false);
	
	return readAndExpectSuccess(this->refWrapper, this->refParser);
}