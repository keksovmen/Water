#include <LiquidCrystal_I2C.h>
#include "SimHandler.h"
#include "SimHandlerHelper.h"
#include "Clock.h"
#include "Device.h"
#include "ParameterHandler.h"
#include "Constants.h"
#include "CardReader.h"


#define BUTTON_TIME 8
#define BUTTON_SEND 9
#define BUTTON_SHOW 10
#define HEATER_PIN 53

//Barrometr, termometr
Device dev;

//Sim module
#ifdef _AVR_ATMEGA328PB_H_INCLUDED
	#include <SoftwareSerial.h>
	SoftwareSerial sim (2, 3);
#elif defined (_AVR_IOM2560_H_)
	#include <HardwareSerial.h>
	HardwareSerial& sim = Serial1;
#endif


//Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

ParameterHandler parameters;

//Time
Clock& clk = parameters.getClock().getValue();


//NFC
CardReader cardReader(3, Serial2, lcd, parameters.getCard().getValue());


//For proper calculation of time
unsigned long timeBefore;


// SimHandler<FIXED_BUFFER_SIZE> simHandler(sim, parameters);
SimHandlerHelper<FIXED_BUFFER_SIZE> simHelper(sim, parameters);
SimHandler& simHandler = simHelper.getHandler();




bool waitForResult(const char* str = "Sending data");



void setup(){
	Serial.begin(115200);
	sim.begin(115200);
	
	//Buttons, as input
	pinMode(BUTTON_TIME, INPUT);
	pinMode(BUTTON_SEND, INPUT);
	pinMode(BUTTON_SHOW, INPUT);
	pinMode(HEATER_PIN, OUTPUT);
	
	
	//Set them on HIGH, to short after
	digitalWrite(BUTTON_TIME, HIGH);
	digitalWrite(BUTTON_SEND, HIGH);
	digitalWrite(BUTTON_SHOW, HIGH);
	digitalWrite(HEATER_PIN, LOW);
	
	
	lcd.init();
	lcd.backlight();
	
	
	
	//Check if device is connected
	if(!dev.init()){
		Serial.println("Device not connected");
		// while(1){};
	}
	
	if(!cardReader.init()){
		Serial.println("NFC not connected");
	}
	
	// if(!simHelper.init()){
		// delay(3000);
		// if(!simHelper.init()){
			// while(1){}
		// }
	// }
	
	//update time
	timeBefore = millis();
	
	//set IP
	parameters.getAddress().getValue().parse("37.146.144.72");
	parameters.getApn().getValue().parse("internet");
	parameters.getTempUp().getValue().parse("40");
	parameters.getTempDown().getValue().parse("30");
	parameters.getPlateId().getValue().parse("-1");
	
	// sim.write("AT+CGATT=0\r");
	// delay(3000);
	// sim.read();
}

void loop(){
	//calculate delta time
	unsigned long timeNow = millis();
	int timePassed = timeNow - timeBefore;
	timeBefore = timeNow;
	
	//check if summ of millis exceeded 1 sec
	if(clk.addMillis(timePassed)){
		printTime(clk);
	}
	
	
	// if button 8 pressed
	if(digitalRead(BUTTON_TIME) == LOW){
		// printMessage("Asking time");
		if(!simHelper.isAbleToUseHttp()){
			printMessage("HTTP UNAVAILABLE");
			delay(1000);
		}else{
			askTime();
		}
	}
	
	
	//if button 9 pressed
	if(digitalRead(BUTTON_SEND) == LOW){
		if(!simHelper.isAbleToUseHttp()){
			printMessage("HTTP UNAVAILABLE");
			delay(1000);
		}else{
			sendSensorData();
		}
		
	}
	
	
	//if button 10 pressed
	// if(digitalRead(BUTTON_SHOW) == LOW){

	// }
	
	
	simHandler.doActivity();
	
	if(cardReader.read()){
		if(!simHelper.isAbleToUseHttp()){
			printMessage("HTTP UNAVAILABLE");
			delay(1000);
		}else{
			if(askVolume()){
				if(parameters.getUserVolume().getValue().getValue() == 0){
					printMessage("Zero Volume");
					delay(1000);
				}else{
					if(cardReader.readCard(parameters.getUserVolume().getValue().getValue())){
						parameters.getGivenVolume().getValue().getValue() = cardReader.getCounter();
						if(parameters.getGivenVolume().getValue().getValue() > 0){
							sendVolume();
							printMessage("Sended");
							delay(2000);
						}
					}
				}
			}else{
				printMessage("Server Error");
				delay(1000);
			}
		}

	}
	
	
	handleTemperature();
	
}


void handleTemperature(){
	updateParams();
	
	double tU = parameters.getTempUp().getValue().getValue();
	double tD = parameters.getTempDown().getValue().getValue();
	double tC = parameters.getTemp().getValue().getValue();
	
	if(tC > tU){
		digitalWrite(HEATER_PIN, LOW);
	}
	
	if(tC < tD){
		digitalWrite(HEATER_PIN, HIGH);
	}
	
}


//Pretty time format printing
void printTime(const Clock& clk){
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


//Tries to send temperature and pressure
//to server, also displays what was send
void sendSensorData(){
	updateParams();
	if(!simHelper.sendParams()){
		printMessage("Network Error");
		delay(1000);
		return;
	}
	
	waitForResult();
	
	showEntry(
		parameters.getTemp().getValue().getValue(),
		parameters.getPressure().getValue().getValue()
		);
}


bool askVolume(){
	// printMessage("Asking Volume");
	if(!simHelper.askVolume()){
		printMessage("Network Error");
		delay(1000);
		return false;
	}
	
	const char* str = "Asking Volume";
	
	printMessage(str);
	lcd.setCursor(strlen(str), 0);
	
	unsigned long t = millis();
	bool b = false;
	
	while(simHelper.isAnwserRdy() == 0){
		if(!cardReader.read()){
			simHelper.abort();
			return false;
		}
		
		if((millis() - t) > 500){
			t = millis();
			lcd.setCursor(strlen(str), 0);
			lcd.print("  ");
			lcd.setCursor(strlen(str), 0);
			
			if(b)
				lcd.print("..");
			else
				lcd.print(".");
			
			b = !b;
		}
	}
	
	if(!simHelper.isAnwserSuccess()){
		printMessage("Server Fail");
		delay(1000);
		return false;
	}
	
	return true;
	
	// return waitForResult("Asking Volume");
}


void sendVolume(){
	// parameters.getGivenVolume().getValue().getValue() = cardReader.getCounter();
		
	if(!simHelper.sendVolume()){
		printMessage("Network Error");
		delay(1000);
		return;
	}
	
	waitForResult("Sending Volume");
}


void askTime(){
	if(!simHelper.askTime()){
		printMessage("Network Error");
		delay(1000);
		return;
	}
	
	waitForResult("Asking Time");
}


//Display temperature and pressure values for some time
void showEntry(int temp, int pressure){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("TEMP = ");
	lcd.setCursor(8,0);
	lcd.print(temp);
	
	lcd.setCursor(0,1);
	lcd.print("PRES = ");
	lcd.setCursor(8,1);
	lcd.print(pressure);
	
	delay(1500);
}


//CAUTION max length of str is 16 symbols
void printMessage(const char* str){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(str);
}

void updateParams(){
	dev.readResults();
	double temp = dev.getTemperature();
	double press = dev.getPressure();
	parameters.getTemp().getValue().getValue() = temp;
	parameters.getPressure().getValue().getValue() = press;
}

bool waitForResult(const char* str){
	printMessage(str);
	
	unsigned long t = millis();
	bool b = false;
	
	//TODO: when module dies it loops forever
	while(simHelper.isAnwserRdy() == 0){
		if((millis() - t) > 500){
			t = millis();
			lcd.setCursor(strlen(str), 0);
			lcd.print("  ");
			lcd.setCursor(strlen(str), 0);
			
			if(b)
				lcd.print("..");
			else
				lcd.print(".");
			
			b = !b;
		}
	}
	
	if(!simHelper.isAnwserSuccess()){
		printMessage("Server Fail");
		delay(1000);
		return false;
	}
	
	return true;
}