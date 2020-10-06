#include "Util.h"



int characterToInt(char c){
	return c - '0';
}


int findLongLength(long val){
	long delimeter = 1000000000;
	bool started = false;
	int count = 0;

	if(val < 0){
		val = abs(val);
		count++;	//minus sign
	}
	
	while(delimeter > 0){
		if ((val / delimeter) > 0){
			started = true;
			val -= delimeter;
		}
		
		if(started){
			count++;
		}
		
		delimeter /= 10;
	}
	
	return count;
}


int findDoubleLength(double val, int afterDot){
	return findLongLength(val) + afterDot + 1;	//1 for dot character
}