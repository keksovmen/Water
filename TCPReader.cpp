#include "TCPReader.h"
#include "Util.h"
#include "Enums.h"
#include "StringData.h"



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
	return 25 + findLongLength(this->responceLength - this->readIndex);
}


template<int N>
void TCPReader<N>::removeGarbage(){
	this->refParser.removeReadGarbage(READ_TYPE_TCP);
}


template<int N>
bool TCPReader<N>::isMessageFull(){
	return this->refParser.isReadMessageFull(READ_TYPE_TCP);
}


template<int N>
void TCPReader<N>::askForData(int index, int amount){
	this->refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_GET_NORMAL, amount);
}