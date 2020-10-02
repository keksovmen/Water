#include "PostDataHandler.h"
#include "Util.h"


template <int N>
PostDataHandler<N>::PostDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer) :
	DataHandler<N>(wrapper, parser, writer){
		
}


template<int N>
bool PostDataHandler<N>::send(){
	this->refWrapper.writeEndOfCommand();
	
	if(!readAndExpectSuccess(this->refWrapper, this->refParser)){
		return false;
	}
		
	this->refWriter.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	return readAndExpectSuccess(this->refWrapper, this->refParser);
}
