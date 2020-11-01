#include "TCPReader.h"
#include "Util.h"
#include "Enums.h"
#include "StringData.h"




TCPReader::TCPReader(	SimResultParser& parser, 
						SimCommandPort& simPort,
						FixedBufferBase& buffer,
						unsigned long length
						) :
	ResponceReader(parser, simPort, buffer)
{
	this->responceLength = length;
}



int TCPReader::getMinMessageLength(){
	return 25 + findLongLength(this->responceLength - this->readIndex);
}



void TCPReader::removeGarbage(){
	this->refParser.removeReadGarbage(READ_TYPE_TCP);
}



bool TCPReader::isMessageFull(){
	return this->refParser.isReadMessageFull(READ_TYPE_TCP);
}



void TCPReader::askForData(int index, int amount){
	this->refPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_GET_NORMAL, amount);
}