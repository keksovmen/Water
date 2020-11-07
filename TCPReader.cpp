#include "TCPReader.h"
#include "Util.h"
#include "Enums.h"
#include "StringData.h"




TCPReader::TCPReader(	SimTools& tools,
						FixedBufferBase& buffer,
						unsigned long length
						) :
	ResponceReader(tools, buffer)
{
	this->responceLength = length;
}



int TCPReader::getMinMessageLength(){
	return 25 + findLongLength(this->responceLength - this->readIndex);
}



void TCPReader::removeGarbage(){
	this->refTools.parser.removeReadGarbage(READ_TYPE_TCP);
}



bool TCPReader::isMessageFull(){
	return this->refTools.parser.isReadMessageFull(READ_TYPE_TCP);
}



void TCPReader::askForData(int index, int amount){
	this->refTools.simPort.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_GET_NORMAL, amount);
}