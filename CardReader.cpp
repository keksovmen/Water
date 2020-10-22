#include "CardReader.h"

#include <Arduino.h>

//ISR
void count();
//interrupt will change it
static volatile int counter;



CardReader::CardReader(int interruptPin, HardwareSerial& port, LiquidCrystal_I2C& display) : 
	interruptPin (interruptPin), pn532hsu(port),
	nfc(pn532hsu), display(display)
{
	
}


bool CardReader::init(){
	pinMode(interruptPin, INPUT);
	digitalWrite(interruptPin, HIGH);
	
	nfc.begin();
	
	if(!nfc.getFirmwareVersion()){
		return false;
	}
	
	nfc.setPassiveActivationRetries(0xFF);
	nfc.SAMConfig();
	
	return true;
}


bool CardReader::readCard(){
	if(read()){
		counter = 0;
		printCounter();
		
		int oldVal = counter;
		
		//attach interrupt
		attachInterrupt(digitalPinToInterrupt(interruptPin), count, FALLING);
		
		while(read()){
			if(oldVal != counter){
				printCounter();
				oldVal = counter;
			}
		}
		//detach interrupt
		detachInterrupt(digitalPinToInterrupt(interruptPin));
		
		// counter = 0;
		// display.clear();
		
		return true;
	}
	
	return false;
}


int CardReader::getCounter(){
	return counter;
}


bool CardReader::read(){
	boolean success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
	uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50);
	if(nfc.inRelease(0) < 0){
		Serial.println("Failed to release nfc");
	}
	
	return success;
}

void CardReader::printCounter(){
	display.clear();
	display.setCursor(0, 0);
	display.print("Counter = ");
	display.setCursor(0, 1);
	display.print(counter);
}


//Interrupt
void count(){
	counter++;
}