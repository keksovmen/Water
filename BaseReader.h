#pragma once


class BaseReader
{
	public:
		
		virtual bool read() = 0;
		virtual bool readTimeout(unsigned long maxDelay) = 0;
		
};
