#pragma once


/**
	@param c must be in range 0...9
*/

int characterToInt(char c);


/**
	Finds length in characters of given number
	
	@param val
	@return length of val
*/

int findLongLength(long val);


/**
	Find length in characters of given float point number
	
	@param val
	@param afterDot amount of digits after dot
	
	@return length of val + afterDot
*/

int findDoubleLength(double val, int afterDot);

