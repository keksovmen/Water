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
		printMessage("Asking time");
		simHelper.askTime();
	}
	
	
	//if button 9 pressed
	if(digitalRead(BUTTON_SEND) == LOW){
		printMessage("Sending data");
		sendSensorData();
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
		Parameter<PrimitivIntParameter<int>> volume(4);
		volume.getValue().getValue() = cardReader.getCounter();
		simHelper.sendVolume(volume, parameters);
	}
	
}


//Tries to get time frome server
//And update current clock
// void askForTime(){
	// if(!checkSimModuleReady()){
		// Serial.println("ASK TIME FAILED!");
		// return;
	// }
	
	// DataHandler<FIXED_BUFFER_SIZE>* getHandler = simHandler.sendGetRequest();
	
	// if(!getHandler){
		// Serial.println("Handler is null");
		// return;
	// }
	
	// getHandler->write("http://128.69.240.186/GetTime.php");
	
	// if(getHandler->send()){
		// while(!getHandler->isSended()){
			// delay(1000);
			// Serial.println("Send took 1 sec");
		// }
	// }else{
		// Serial.println("SEND FAILED");
		// getHandler->finish();
		// return;
	// }
	
	// if(!getHandler->isSendedSuccesfully()){
		// Serial.println("SEND FINISHED NO SUCCESS CODE");
		// getHandler->finish();
		// return;
	// }
	
	// getHandler->getBuffer().clear();
	// if(getHandler->readResponce()){
		// auto& b = getHandler->getBuffer();
		// if(!clk.parse(b.begin())){
			// Serial.println("CLOCK PARSE FAILED BUFFER:");
			// Serial.println(b.begin());
			// getHandler->finish();
			// return;
		// }
	// }
	
	// getHandler->finish();
	
	// if(!simHandler.disconnectFromGPRS()){
		// Serial.println("Failed close GPRS");
	// }
// }


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
	simHelper.sendParams(parameters);
	
	showEntry(
		parameters.getTemp().getValue().getValue(),
		parameters.getPressure().getValue().getValue()
		);
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


//Tries to get raw sensor data from server
//Also displays it
// void askServerData(){
	// if(!checkSimModuleReady()){
		// Serial.println("ASK DATA FAILED!");
		// return;
	// }
	
	// DataHandler<FIXED_BUFFER_SIZE>* getHandler = simHandler.sendGetRequest();
	
	// if(!getHandler){
		// Serial.println("Handler is null");
		// return;
	// }
	
	// getHandler->write("http://128.69.240.186/ReadRaw.php");
	
	// if(getHandler->send()){
		// while(!getHandler->isSended()){
			// delay(1000);
			// Serial.println("Send took 1 sec");
		// }
	// }else{
		// Serial.println("SEND FAILED");
		// getHandler->finish();
		// return;
	// }
	
	// if(!getHandler->isSendedSuccesfully()){
		// Serial.println("SEND FINISHED NO SUCCESS CODE");
		// getHandler->finish();
		// return;
	// }
	
	// getHandler->getBuffer().clear();
	// while(getHandler->readResponce()){
		// auto& b = getHandler->getBuffer();
		// int index;
		
		// while((index = b.indexOf("\n")) != -1){
			// if(index == -1)
				// continue;
			
			// int startIndex = b.indexOf("Temperature");
			// int temp = atof(&b[startIndex + 17]);
			
			// startIndex = b.indexOf("Pressure");
			// int pressure = atof(&b[startIndex + 15]);
			
			// showEntry(temp, pressure);
			
			// startIndex = b.indexOf("Time: ");
			// if(startIndex != -1){
				// Clock tmpClock;
				// if(tmpClock.parse(&b[startIndex + 6])){
					// printTime(tmpClock);
					// delay(1500);
				// }
				
			// }
			
			// b.substring(index);	//substring from /nTemperature of new line
			// b.remove(0, 1);	//delete /n symbol
			
		// }
		
	// }
	
	// getHandler->finish();
	
	// if(!simHandler.disconnectFromGPRS()){
		// Serial.println("Failed close GPRS");
	// }
// }


//Check if sim module is working and tries to open GPRS connection
// bool checkSimModuleReady(){
	// if(!simHandler.isModuleUp()){
		// delay(5000);
		// if(!simHandler.isModuleUp()){
			// Serial.println("Module offline");
			// return false;
		// }
	// }
	
	// if(!simHandler.isModuleAlive()){
		// Serial.println("Module not alive");
			// return false;
	// }
	
	// if(simHandler.isConnectedToNetwork() != 
						// NETWORK_CONNECTION::REGISTERED){
		// delay(5000);
		// if(simHandler.isConnectedToNetwork() != 
						// NETWORK_CONNECTION::REGISTERED){
			// Serial.println("Can't connect to network");
			// return false;
		// }
	// }
	
	// if(!simHandler.connectToGPRS("internet")){
		// delay(5000);
		// if(!simHandler.connectToGPRS("internet")){
			// Serial.println("Can't connect to GPRS");
			// return false;
		// }
	// }
	
	// return true;
// }


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