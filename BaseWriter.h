#pragma once

class BaseWriter
{
	public:
		virtual void writeString(const char* str) = 0;
		virtual void writeChar(char c) = 0;
		virtual void writeInt(int i) = 0;
		virtual void writeDouble(double d, int amountAfterDot) = 0;
};
