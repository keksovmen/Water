#include "GetDataHandler.h"
#include "Util.h"


template <int N>
GetDataHandler<N>::GetDataHandler(CommandWriter& wrapper, 
									SimResultParser<N>& parser, 
									SimCommandWriter& writer,
									BaseReader& reader,
									FixedBuffer<N>& buffer) :
	DataHandler<N>(wrapper, parser, writer, reader, buffer){
		
}


template<int N>
bool GetDataHandler<N>::send(){
	this->refWriteHandler.write('"');
	this->refWriteHandler.writeEndOfCommand();
			
	if(!readAndExpectSuccess(this->refReader, this->refParser)){
		return false;
	}
		
	this->refWriter.writeHTPPAction(HTTP_REQUESTS::HTTP_GET);
	
	return readAndExpectSuccess(this->refReader, this->refParser);
}