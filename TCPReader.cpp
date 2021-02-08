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
	return strlen(TCP_READ_ANWSER) + 
			strlen(TEXT_SUCCESS) +
			findLongLength(this->responceLength - this->readIndex) +
			4;
}



void TCPReader::removeGarbage(){
	this->refTools.removeReadGarbage(READ_TYPE_TCP);
}



bool TCPReader::isMessageFull(){
	return this->refTools.isReadMessageFull(READ_TYPE_TCP);
}



void TCPReader::askForData(int index, int amount){
	this->refTools.writeCIPRXGET(CIPRXGET_COMMAND::CIPRXGET_COMMAND_GET_NORMAL, amount);
}