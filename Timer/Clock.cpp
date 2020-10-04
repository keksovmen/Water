#ifndef ARDUINO
	#include <stdlib.h>
	#include <cstring>
#endif

#include "Clock.h"


/**
	Expetcs data as:
	dd/MM/yy
	hh/mm//ss
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

void Clock::addMillis(int mil){
	millis += mil;
	while(millis >= 1000){
		addSeconds(1);
		millis -= 1000;
	}
}


void Clock::addSeconds(int sec){
	seconds += sec;
	while(seconds >= 60){
		addMinutes(1);
		seconds -= 60;
	}
}


void Clock::addMinutes(int min){
	minutes += min;
	while(minutes >= 60){
		addHours(1);
		minutes -= 60;
	}
}


void Clock::addHours(int hour){
	hours += hour;
	while(hours >= 24){
		addDays(1);
		hours -= 24;
	}
}


void Clock::addDays(int day){
	days += day;
	while(days > findMothDays(months)){
		days -= findMothDays(months);
		addMonths(1);
	}
}


void Clock::addMonths(int month){
	months += month;
	while(months > 12){
		addYears(1);
		months -= 12;
	}
}


void Clock::addYears(int year){
	years += year;
	while(years >= 100){
		years -= 100;
	}
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