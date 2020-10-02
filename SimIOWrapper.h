#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"


template<int N>
class SimIOWrapper
{
	public:
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		void writeCommand(const char* cmd, bool clearBuffer = true);
		
		SimIOWrapper<N>& writeChar(char c);
		SimIOWrapper<N>& writeInt(int c);
		SimIOWrapper<N>& writeString(const char* c);
		
		void writeEndOfCommand(bool clearBuffer = true);
		
		
		bool readToBuffer();
		bool readToBufferTimeout(int millis);
		
		
		FixedBuffer<N>& getBuffer(){return buffer;};
		
	private:
		bool tryReadToBuffer();
		
		SoftwareSerial& refPort;
		FixedBuffer<N> buffer;
		
};

template class SimIOWrapper<128>;