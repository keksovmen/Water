#include "TCPReader.h"
#include "Util.h"
#include "Enums.h"


template <int N>
TCPReader<N>::TCPReader(	SimResultParser<N>& parser, 
							SimCommandPort& simPort,
							FixedBuffer<N>& buffer,
							unsigned long length
							) :
	ResponceReader<N>(parser, simPort, buffer)
{
	this->responceLength = length;
}


template<int N>
int TCPReader<N>::getMinMessageLength(){
	return 24;
}


template<int N>
void TCPReader<N>::removeGarbage(){
	this->refParser.removeReadTCPGarbage();
}


template<int N>
bool TCPReader<N>::isMessageFull(){
	return this->refParser.isReadTCPMessageFull();
}


template<int N>
void TCPReader<N>::askForData(int index, int amount){
	this->refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_GET_NORMAL, amount);
}