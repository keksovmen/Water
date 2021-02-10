#include "LCDHelper.h"
#include <Arduino.h>


LCDHelper::LCDHelper(
				int address,
				int width,
				int height,
				ParameterHandler& parameters
				) : 
	lcd (address, width, height),
	width(width),
	height(height),
	displayLength(width * height),
	refParams(parameters)
{
	lcd.init();
	lcd.backlight();
	lcd.autoscroll();
}


void LCDHelper::doActivity(){
	if(timer.isOpen()){
		printTime(refParams.getClock().getValue());
	}
}


void LCDHelper::print(const char* str, unsigned long time){
	lcd.clear();
	lcd.setCursor(0, 0);
	
	int length = strlen(str);
	//TODO: it only support 2 lines modules
	if(length > width){
		int index = findSpaceIndex(str);
		
		for(int i = 0; i < index; i++){
			lcd.print(str[i]);
		}
		lcd.setCursor(0, 1);
		
		//pass space character
		index++;
		for(int i = index; i < displayLength; i++){
			if(i == length){
				break;
			}
			lcd.print(str[i]);
		}
	}else{
		lcd.print(str);
	}
	
	timer.sheduleDelay(time);
}


void LCDHelper::print(const char* first, const char* second, unsigned long time){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(first);
	
	lcd.setCursor(0, 1);
	lcd.print(second);
	
	timer.sheduleDelay(time);
}


void LCDHelper::printTime(const Clock& clk){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Date: ");
	lcd.print(clk.getDays());
	lcd.print(":");
	lcd.print(clk.getMonths());
	lcd.print(":");
	lcd.print(clk.getYears());
	
	lcd.setCursor(0, 1);
	lcd.print("Time: ");
	lcd.print(clk.getHours());
	lcd.print(":");
	lcd.print(clk.getMinutes());
	lcd.print(":");
	lcd.print(clk.getSeconds());
}


int LCDHelper::findSpaceIndex(const char* str){
	int index = width - 1;
	for(int i = index; i >= 0; i--){
		if(str[i] == ' '){
			index = i;
			return index;
		}
	}
	return index;
}