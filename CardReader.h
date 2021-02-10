#pragma once

#include <PN532_HSU.h>
#include <PN532.h>
#include <HardwareSerial.h>
#include "CardParameter.h"
#include "LCDHelper.h"


class CardReader
{
	public:
		CardReader(
				int interruptPin,
				int waterValvePin,
				HardwareSerial& port, 
				LCDHelper& display,
				CardParameter& card
				);
		
		bool init();
		bool readCard(int volume);
		
		
		int getCounter();
		
		bool read();
		
	private:
		void printCounter();
	
		const int interruptPin;
		const int waterValvePin;
		PN532_HSU pn532hsu;
		PN532 nfc;
		
		LCDHelper& display;
		CardParameter& refCard;
	
};