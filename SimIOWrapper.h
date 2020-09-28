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
		
		bool writeCommand(const String& cmd);
		
		bool readToBuffer();
		
		FixedBuffer<N>& getBuffer(){return buffer;};
		
	private:
		SoftwareSerial& refPort;
		FixedBuffer<N> buffer;
		
};

template class SimIOWrapper<128>;