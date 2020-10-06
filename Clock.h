#pragma once

#ifdef ARDUINO
	#include "BaseParameter.h"
#endif

#ifdef ARDUINO
	class Clock : public BaseParameter
#else 
	class Clock
#endif
{
	public:
		
		bool parse(const char* str);
		
		bool addMillis(int mil);
		bool addSeconds(int sec);
		bool addMinutes(int min);
		bool addHours(int hour);
		bool addDays(int dda);
		bool addMonths(int month);
		bool addYears(int year);
		
		int getMillis(){return millis;};
		int getSeconds(){return seconds;};
		int getMinutes(){return minutes;};
		int getHours(){return hours;};
		int getDays(){return days;};
		int getMonths(){return months;};
		int getYears(){return years;};
		
		#ifdef ARDUINO
			int getLength() override;
			void handleWritingValue(BaseWriter& writer) override;
		#endif
		
	private:
		int findMothDays(int month);
	
		int days = 1;
		int months = 1;
		int years = 20;
		
		int hours = 0;
		int minutes = 0;
		int seconds = 0;
		
		int millis = 0;
};