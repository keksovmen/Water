#pragma once

#include "BaseWriter.h"

class CommandWriter : public BaseWriter
{
	public:
		virtual void writeCommand(const char* cmd, bool clearBuffer = true){
			write(cmd);
			writeEndOfCommand(clearBuffer);
		};
		
		virtual void writeEndOfCommand(bool clearBuffer = true) = 0;
		
};
