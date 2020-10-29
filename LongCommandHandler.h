#pragma once


class LongCommandHandler
{
	public:
	
		/**
			@return true only if command get what expected
		*/
		
		virtual bool handle() = 0;
	
};