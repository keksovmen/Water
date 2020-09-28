#include <Ardhuino.h>
#include "SimFacade.h"
#include "Enums.h"

template<int N>
SimFacade<N>::SimFacade(SoftwareSerial& refPort) :
wrapper(refPort)
{
	
}

template<int N>
bool SimFacade<N>::isModuleUp(){
	writer.writeAT(wrapper);
	if(wrapper.readToBuffer()){
		if(parser.isSimpleMessageReady(wrapper.getBuffer())){
			if(parser.fetchResultCode(wrapper.getBuffer())
					== ANWSER_CODES::OK)
			{
				return true;
			}
		}else{
			//TODO: Coud fail here due to Unsolicited messages
			//UNDER VOLTAG etc. fix it, through parser
			return isModuleUp();
		}
	}
	
	return false;
}