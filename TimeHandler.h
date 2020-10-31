#pragma once


class TimeHandler
{
	public:
	
		/**
			@param duration in millis
		*/
	
		void sheduleDelay(unsigned long duration);
		
		
		/**
			@return true if isSheduled = false
		*/
		
		bool isOpen();
		
	private:
		unsigned long delayDuration;
		unsigned long startTime;
		bool isSheduled = false;
	
};