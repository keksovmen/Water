#ifdef ARDUINO
	#include <Arduino.h>
	#include "Util.h"
#else
	#include <stdlib.h>
	#include <cstring>
#endif

#include "Clock.h"


/**
	Expetcs data as:
	dd/MM/yy|hh/mm/ss
*/

bool Clock::parse(const char* str){
	int length = strlen(str);
	if(length < 17)
		return false;
	
	days = atoi(str);
	months = atoi(str + 3);
	years = atoi(str + 6);
	hours = atoi(str + 9);
	minutes = atoi(str + 12);
	seconds = atoi(str + 15);
	millis = 0;
	
	return true;
}

bool Clock::addMillis(int mil){
	bool r = false;
	
	millis += mil;
	while(millis >= 1000){
		addSeconds(1);
		millis -= 1000;
		
		r = true;
	}
	
	return r;
}


bool Clock::addSeconds(int sec){
	bool r = false;
	
	seconds += sec;
	while(seconds >= 60){
		addMinutes(1);
		seconds -= 60;
		
		r = true;
	}
	
	return r;
}


bool Clock::addMinutes(int min){
	bool r = false;
	
	minutes += min;
	while(minutes >= 60){
		addHours(1);
		minutes -= 60;
		
		r = true;
	}
	
	return r;
}


bool Clock::addHours(int hour){
	bool r = false;
	
	hours += hour;
	while(hours >= 24){
		addDays(1);
		hours -= 24;
		
		r = true;
	}
	
	return r;
}


bool Clock::addDays(int day){
	bool r = false;
	
	days += day;
	while(days > findMothDays(months)){
		days -= findMothDays(months);
		addMonths(1);
		
		r = true;
	}
	
	return r;
}


bool Clock::addMonths(int month){
	bool r = false;
	
	months += month;
	while(months > 12){
		addYears(1);
		months -= 12;
		
		r = true;
	}
	
	return r;
}


bool Clock::addYears(int year){
	bool r = false;
	
	years += year;
	while(years >= 100){
		years -= 100;
		
		r = true;
	}
	
	return r;
}


int Clock::findMothDays(int month){
	switch(month){
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
			
		// Trap for years, because only 2 digits
		//can't properly find february days need check for
		// % 100 for 28 days, %400 for 29 days
		case 2:
			if((years % 4) == 0)
				return 29;
			else
				return 28;
		
		default: return -1;
	}
}


#ifdef ARDUINO

int Clock::getLength(){
	return 17;	// 5 for delimiters, 2 for each value
}

void Clock::handleWritingValue(BaseWriter& writer){
	if(days < 10)
		writer.write(0);
	writer.write(days);
	writer.write(':');
	
	if(months < 10)
		writer.write(0);
	writer.write(months);
	writer.write(':');
	
	if(years < 10)
		writer.write(0);
	writer.write(years);
	writer.write(':');
	
	if(hours < 10)
		writer.write(0);
	writer.write(hours);
	writer.write(':');
	
	if(minutes < 10)
		writer.write(0);
	writer.write(minutes);
	writer.write(':');
	
	if(seconds < 10)
		writer.write(0);
	writer.write(seconds);
}

#endif