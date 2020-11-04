#pragma once

#include <PN532_HSU.h>
#include <PN532.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>


class CardReader
{
	public:
		CardReader(
				int interruptPin, 
				HardwareSerial& port, 
				LiquidCrystal_I2C& display
				);
		
		bool init();
		bool readCard(int volume);
		
		
		int getCounter();
		
		bool read();
		
	private:
		void printCounter();
	
		const int interruptPin;
		PN532_HSU pn532hsu;
		PN532 nfc;
		
		LiquidCrystal_I2C& display;
	
};