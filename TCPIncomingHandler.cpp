#include "TCPIncomingHandler.h"
#include <Arduino.h>


template<int N>
void TCPIncomingHandler<N>::handleMessage(
		FixedBuffer<N>& buffer, 
		ParameterHandler& params,
		SimCommandPort& simPort,
		SimResultParser<N>& parser 
		)
{
	int index = 0;
	int cmd = findCommand(buffer, index);
	
	while(cmd != -1){
		// Serial.print("CMD = ");
		// Serial.println("BUFFER BEFORE:");
		// Serial.println(buffer.begin());
		// Serial.println(cmd);
		
		handleCommand(buffer, cmd, index, 
						params, simPort, parser);
		removeCommand(buffer, cmd, index);
		
		// Serial.println("BUFFER AFTER:");
		// Serial.println(buffer.begin());
		
		cmd = findCommand(buffer, index);
	}
}


template<int N>
int TCPIncomingHandler<N>::findCommand(
			FixedBuffer<N>& buffer, 
			int& returnIndex
			)
{
	int end = -1;
	returnIndex = buffer.indexOf("2=");
		if(returnIndex != -1){
			end = buffer.indexOfFrom(returnIndex, "\n");
		if(end != -1){
			return 1;
		}
	}
	
	returnIndex = buffer.indexOf("PING");
		if(returnIndex != -1){
			end = buffer.indexOfFrom(returnIndex, "\n");
		if(end != -1){
			return 2;
		}
	}
	
	return -1;
}


template<int N>
void TCPIncomingHandler<N>::removeCommand(
			FixedBuffer<N>& buffer, 
			int cmd, 
			int index
			)
{
	int end = buffer.indexOfFrom(index, "\n");
	if(end != -1){
		int amount = end - index;
		amount++;
		
		buffer.remove(index, amount);
	}
}


template<int N>
void TCPIncomingHandler<N>::handleCommand(
			FixedBuffer<N>& buffer, 
			int cmd, 
			int index,
			ParameterHandler& params,
			SimCommandPort& simPort,
			SimResultParser<N>& parser
			)
{
	switch(cmd){
		case 1:
			handleClockUpdate(buffer, index + 2, params);
			break;
		case 2:
			handlePing(simPort, parser, buffer);
			break;
		
		default: break;
	}
}


template<int N>
void TCPIncomingHandler<N>::handleClockUpdate(
			FixedBuffer<N>& buffer, 
			int index,
			ParameterHandler& params
			)
{
	params.getClock().getValue().parse(&buffer[index]);
}


template<int N>
void TCPIncomingHandler<N>::handlePing(
			SimCommandPort& simPort, 
			SimResultParser<N>& parser,
			FixedBuffer<N>& buffer
			)
{
	simPort.writeCIPSEND();
	if(!simPort.read()){
		return;
	}
	
	if(!buffer.remove("\r\n>")){
		return;
	}
	
	simPort.write("PONG\n");
	simPort.write((char) 0x1A);
	
	simPort.readTimeout(5000);
}