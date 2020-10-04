#pragma once

class Clock
{
	public:
		
		bool parse(const char* str);
		
		void addMillis(int mil);
		void addSeconds(int sec);
		void addMinutes(int min);
		void addHours(int hour);
		void addDays(int dda);
		void addMoths(int moth);
		void addYears(int year);
		
		int getMillis(){return millis;};
		int getSeconds(){return seconds;};
		int getMinutes(){return minutes;};
		int getHours(){return hours;};
		int getDays(){return days;};
		int getMoths(){return moths;};
		int getYears(){return years;};
		
	private:
		int findMothDays(int moth);
	
		int days = 1;
		int moths = 1;
		int years = 20;
		
		int hours = 0;
		int minutes = 0;
		int seconds = 0;
		
		int millis = 0;
};