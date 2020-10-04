#include "Clock.h"
#include <assert.h>
#include <iostream>


void checkAddMillis();
void checkAddMinutes();
void checkAddHours();
void checkAddDays();
void checkAddMonths();
void checkAddYears();
void checkParse();

int main(){
	checkAddMillis();
	checkAddMinutes();
	checkAddHours();
	checkAddDays();
	checkAddMonths();
	checkAddYears();
	checkParse();
}


void checkAddMillis(){
	Clock c;
	c.addMillis(500);
	assert (c.getMillis() == 500);
	
	c.addMillis(750);
	assert (c.getMillis() == 250);
	assert (c.getSeconds() == 1);
}


void checkAddMinutes(){
	Clock c;
	c.addMinutes(30);
	assert (c.getMinutes() == 30);
	
	c.addMinutes(75);
	assert (c.getMinutes() == 45);
	assert (c.getHours() == 1);
}


void checkAddHours(){
	Clock c;
	c.addHours(12);
	assert (c.getHours() == 12);
	
	c.addHours(36);
	assert (c.getHours() == 0);
	assert (c.getDays() == 3);
}


void checkAddDays(){
	Clock c;
	c.addDays(12);
	assert (c.getDays() == 13);
	
	c.addDays(31);
	assert (c.getDays() == 13);
	assert (c.getMonths() == 2);
}


void checkAddMonths(){
	Clock c;
	c.addMonths(6);
	assert (c.getMonths() == 7);
	
	c.addMonths(26);
	assert (c.getMonths() == 9);
	assert (c.getYears() == 22);
}


void checkAddYears(){
	Clock c;
	c.addYears(6);
	assert (c.getYears() == 26);
	
	c.addYears(230);
	assert (c.getYears() == 56);
}


void checkParse(){
	Clock c;
	assert (c.parse("04/10/20\n14/09/53"));
	
	assert(c.getDays() == 4);
	assert(c.getMonths() == 10);
	assert(c.getYears() == 20);
	assert(c.getHours() == 14);
	assert(c.getMinutes() == 9);
	assert(c.getSeconds() == 53);
	
	assert (c.parse("") == false);
}