#include "PostDataHandler.h"
#include "Util.h"


template <int N>
PostDataHandler<N>::PostDataHandler(CommandWriter& wrapper, 
										SimResultParser<N>& parser, 
										SimCommandWriter& writer,
										BaseReader& reader,
										FixedBuffer<N>& buffer) :
	DataHandler<N>(wrapper, parser, writer, reader, buffer){
		
}


template<int N>
bool PostDataHandler<N>::send(){
	this->refWriteHandler.writeEndOfCommand();
	
	if(!readAndExpectSuccess(this->refReader, this->refParser)){
		return false;
	}
		
	this->refWriter.writeHTPPAction(HTTP_REQUESTS::HTTP_POST);
	
	return readAndExpectSuccess(this->refReader, this->refParser);
}
