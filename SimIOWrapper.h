#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"


template<int N>
class SimIOWrapper
{
	public:
		// explicit SimIOWrapper(SoftwareSerial& refSerial) : 
			// refPort(refSerial){};
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		//TODO: made param as const char* not as String
		//TODO: made writeSendCommand use explicitly
		//TODO: maybe don't return bool result, what to do with it?
		bool writeCommand(const String& cmd);
		
		SimIOWrapper<N>& writeChar(char c);
		SimIOWrapper<N>& writeInt(int c);
		SimIOWrapper<N>& writeString(const char* c);
		
		void writeSendCommand();
		
		
		bool readToBuffer();
		bool readToBufferTimeout(int millis);
		
		FixedBuffer<N>& getBuffer(){return buffer;};
		
	private:
		SoftwareSerial& refPort;
		FixedBuffer<N> buffer;
		
};

template class SimIOWrapper<128>;