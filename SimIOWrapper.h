#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"

//TODO: add template int N to select buffer size
class SimIOWrapper
{
	public:
		// explicit SimIOWrapper(SoftwareSerial& refSerial) : 
			// refPort(refSerial){};
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		bool writeCommand(const String& cmd);
		
		bool readToBuffer();
		
		FixedBuffer<128>& getBuffer(){return buffer;};
		
	private:
		SoftwareSerial& refPort;
		FixedBuffer<128> buffer;
		
};