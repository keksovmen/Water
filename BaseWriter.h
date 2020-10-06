#pragma once

class BaseWriter
{
	public:
		virtual void write(const char* str) = 0;
		virtual void write(char c) = 0;
		virtual void write(int i) = 0;
		virtual void write(long l) = 0;
		virtual void write(double d, int amountAfterDot) = 0;
};
