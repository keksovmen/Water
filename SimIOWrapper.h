#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"


template<int N>
class SimIOWrapper
{
	public:
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		void writeCommand(const char* cmd);
		
		SimIOWrapper<N>& writeChar(char c);
		SimIOWrapper<N>& writeInt(int c);
		SimIOWrapper<N>& writeString(const char* c);
		
		void writeEndOfCommand();
		
		
		bool readToBuffer();
		bool readToBufferTimeout(int millis);
		
		
		FixedBuffer<N>& getBuffer(){return buffer;};
		
	private:
		SoftwareSerial& refPort;
		FixedBuffer<N> buffer;
		
};

template class SimIOWrapper<128>;