#include "PostDataHandler.h"
#include "Util.h"
#include "Enums.h"

template <int N>
PostDataHandler<N>::PostDataHandler(SimIOWrapper<N>& wrapper, SimResultParser<N>& parser, SimCommandWriter<N>& writer) :
	refWrapper (wrapper), refParser(parser), refWriter(writer){
		
}


template<int N>
void PostDataHandler<N>::write(const char* str){
	refWrapper.writeString(str);
}


template<int N>
bool PostDataHandler<N>::send(){
	refWrapper.writeEndOfCommand();
	if(!readAndExpectSuccess(refWrapper, refParser)){
		return false;
	}
		
	refWriter.writeHTPPAction(true);
	
	return readAndExpectSuccess(refWrapper, refParser);
}

template<int N>
bool PostDataHandler<N>::isSended(){
	if(refWrapper.readToBufferTimeout(5000)){
		return refParser.isHttpActionPresents();
	}
	
	return false;
}


template<int N>
bool PostDataHandler<N>::isSendedSuccesfully(){
	//TODO: made enum for codes
	return refParser.fetchHTTPStatus() == 2;
}


template<int N>
void PostDataHandler<N>::finish(){
	refWriter.writeHTPP(HTTP_COMMANDS::HTTP_TERM);
	readAndExpectSuccess(refWrapper, refParser);
}