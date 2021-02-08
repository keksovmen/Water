#pragma once


class BaseReader
{
	public:
		
		/**
			Tries to read and consume some time
		*/
		
		virtual bool read() = 0;
		
		
		/**
			Tries to read for period of time
			
			@param maxDelay period of time
		*/
		
		virtual bool readTimeout(unsigned long maxDelay) = 0;
		
		
		/**
			Tries to read with almost no delay
			Basicly, checks buffer for some data
		*/
		
		virtual bool lazyRead() = 0;
		
};
