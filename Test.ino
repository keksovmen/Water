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
CardReader cardReader(3, Serial2, lcd);


//For proper calculation of time
unsigned long timeBefore;


// SimHandler<FIXED_BUFFER_SIZE> simHandler(sim, parameters);
SimHandlerHelper<FIXED_BUFFER_SIZE> simHelper(sim, parameters);
SimHandler& simHandler = simHelper.getHandler();




void setup(){
	Serial.begin(115200);
	sim.begin(115200);
	
	//Buttons, as input
	pinMode(BUTTON_TIME, INPUT);
	pinMode(BUTTON_SEND, INPUT);
	pinMode(BUTTON_SHOW, INPUT);
	
	
	//Set them on HIGH, to short after
	digitalWrite(BUTTON_TIME, HIGH);
	digitalWrite(BUTTON_SEND, HIGH);
	digitalWrite(BUTTON_SHOW, HIGH);
	
	
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
	parameters.getAddress().getValue().fromString("37.146.149.79");
	
	// sim.write("AT+CGATT=0\r");
	// delay(3000);
	// sim.read();
}

void loop(){
	//calculate delta time
	unsigned long timeNow = millis();
	int timePassed = timeNow - timeBefore;
	timeBefore = timeNow;
	
	// while(1){
		// simHandler.connectToGPRS("internet");
		// simHandler.disconnectFromGPRS();
	// }
	
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
		// printMessage("Asking server");
		// askServerData();
	// }
	
	// simHandler.handleReading();
	simHandler.doActivity();
	
	if(cardReader.readCard()){
		// Serial.print("Counter: ");
		// Serial.println(cardReader.getCounter());
		// Parameter<PrimitivIntParameter<int>> volume(4);
		// volume.getValue().getValue() = cardReader.getCounter();
		// simHelper.sendVolume(volume, parameters);
		if(!simHelper.isAbleToUseHttp()){
			printMessage("HTTP UNAVAILABLE");
			delay(1000);
		}else{
			sendVolume();
		}
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
	if(!simHelper.sendParams(parameters)){
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


void sendVolume(){
	Parameter<PrimitivIntParameter<int>> volume(4);
	volume.getValue().getValue() = cardReader.getCounter();
		
	if(!simHelper.sendVolume(volume, parameters)){
		printMessage("Network Error");
		delay(1000);
		return;
	}
	
	waitForResult();
}


void askTime(){
	if(!simHelper.askTime()){
		printMessage("Network Error");
		delay(1000);
		return;
	}
	
	waitForResult();
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

void waitForResult(){
	printMessage("Sending data");
	unsigned long t = millis();
	bool b = false;
	while(!simHelper.isAnwserRdy()){
		if((millis() - t) > 500){
			t = millis();
			if(b)
				printMessage("Sending data..");
			else
				printMessage("Sending data.");
			b = !b;
		}
	}
	
	if(!simHelper.isAnwserSuccess()){
		printMessage("Server Fail");
		delay(1000);
		return;
	}
}