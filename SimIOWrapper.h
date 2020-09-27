#pragma once
#include <SoftwareSerial.h>
#include "Buffer/FixedBuffer.h"


class SimIOWrapper
{
	public:
		// explicit SimIOWrapper(SoftwareSerial& refSerial) : 
			// refPort(refSerial){};
		explicit SimIOWrapper(SoftwareSerial& refSerial);
		
		void does();
		
	private:
		SoftwareSerial& refPort;
		FixedBuffer<128> buffer;
		
};