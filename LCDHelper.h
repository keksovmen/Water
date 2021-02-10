#pragma once
#include <LiquidCrystal_I2C.h>
#include "Activity.h"
#include "ParameterHandler.h"
#include "TimeHandler.h"



class LCDHelper : public Activity
{
	public:
		const int width;
		const int height;
		const int displayLength;
		
		
		LCDHelper(
				int address,
				int width,
				int height,
				ParameterHandler& parameters
				);
		

		void doActivity() override;
		
		
		/**
			Will automatically print string if it's
			length is greater than max, then max chars will
			be printed
			
			@param str to print
			@param time in millis, for how long to keep writing
		*/
		
		void print(const char* str, unsigned long time = 0);
		
		/**
			Write two string each on its own line
		*/
		
		void print(const char* first, const char* second, unsigned long time = 0);
		

	
	
	private:
		LiquidCrystal_I2C lcd;
		TimeHandler timer;

		
		ParameterHandler& refParams;
		
		void printTime(const Clock& clk);
		int findSpaceIndex(const char* str);
		
		
	
	
};
