#include <LiquidCrystal_I2C.h>
#include "SimHandler.h"
#include "Clock.h"
#include "Device.h"
#include "ParameterHandler.h"


#define BUFFER_SIZE 128

#define BUTTON_TIME 8
#define BUTTON_SEND 9
#define BUTTON_SHOW 10

//Barrometr, termometr
Device dev;

//Sim module
SoftwareSerial sim (2, 3);
SimHandler<BUFFER_SIZE> simHandler(sim);

//Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

ParameterHandler parameters;

//Time
Clock& clk = parameters.getClock().getValue();
// Clock clk;


//For proper calculation of time
unsigned long timeBefore;


void setup(){
	Serial.begin(9600);
	sim.begin(9600);
	
	
	// Serial.println(sizeof(PostDataHandler<BUFFER_SIZE>));
	// Serial.println(sizeof(GetDataHandler<BUFFER_SIZE>));
	// Serial.println(sizeof(DataHandler<BUFFER_SIZE>));
	// Serial.println(sizeof(HTTPHandler<BUFFER_SIZE>));
	
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
		Serial.println("Device is not connected");
		// while(1){};
	}
	
	
	//Check if module is anwserying at least
	if(!simHandler.isModuleUp()){
		delay(5000);
		if(!simHandler.isModuleUp()){
			Serial.println("Module if offline");
			// while(1){};
		}
	}
	
	
	//Check if module is working properly
	if(!simHandler.isModuleAlive()){
		Serial.println("Module is not alive");
		// while(1){};
	}
	
	
	//Set default parameters
	if(!simHandler.setDefaultParams()){
		Serial.println("Defaults are not set");
		// while(1){};
	}
	
	
	//update time
	timeBefore = millis();
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
		printMessage("Asking time");
		askForTime();
	}
	
	
	//if button 9 pressed
	if(digitalRead(BUTTON_SEND) == LOW){
		printMessage("Sending data");
		sendSensorData();
	}
	
	
	//if button 10 pressed
	if(digitalRead(BUTTON_SHOW) == LOW){
		printMessage("Asking server");
		askServerData();
	}
	
	
	//DEBUG
	// if(Serial.available()){
		// sim.write(Serial.read());
	// }
	// if(sim.available()){	
		// Serial.write(sim.read());
	// }
	
}


//Tries to get time frome server
//And update current clock
void askForTime(){
	if(!checkSimModuleReady()){
		Serial.println("ASK FOR TIME FAILED!");
		return;
	}
	
	DataHandler<BUFFER_SIZE>* getHandler = simHandler.sendGetRequest();
	
	if(!getHandler){
		Serial.println("Handler is nullptr");
		return;
	}
	
	getHandler->write("http://128.69.240.186/GetTime.php");
	
	if(getHandler->send()){
		while(!getHandler->isSended()){
			delay(1000);
			Serial.println("Send took 1 sec");
		}
	}else{
		Serial.println("SEND FAILED");
		getHandler->finish();
		return;
	}
	
	if(!getHandler->isSendedSuccesfully()){
		Serial.println("SEND FINISHED WITH NO SUCCESS CODE");
		getHandler->finish();
		return;
	}
	
	getHandler->getBuffer().clear();
	if(getHandler->readResponce()){
		auto& b = getHandler->getBuffer();
		if(!clk.parse(b.begin())){
			Serial.println("CLOCK PARSE FAILED MUST BE DATA CORRAPTION HERE IS BUFFER:");
			Serial.println(b.begin());
			getHandler->finish();
			return;
		}
	}
	
	getHandler->finish();
	
	if(!simHandler.disconnectFromGPRS()){
		Serial.println("Failed to close GPRS");
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
	if(!checkSimModuleReady()){
		Serial.println("SEND SENSOR DATA FAILED");
		return;
	}
	
	
	dev.readResults();
	double temp = dev.getTemperature();
	double press = dev.getPressure();
	parameters.getTemp().getValue().getValue() = temp;
	parameters.getPressure().getValue().getValue() = press;
	
	
	DataHandler<BUFFER_SIZE>* postDataHandler = 
		simHandler.sendPostRequest("http://128.69.240.186/Send.php", parameters.getLength());
		
	if(!postDataHandler){
		Serial.println("Handler is nullptr");
	}
		
	parameters.handleWritingValue((*postDataHandler));
	
	if(postDataHandler->send()){
		while(!postDataHandler->isSended()){
			delay(1000);
			Serial.println("Send took 1 sec");
		}
	}else{
		Serial.println("SEND FAILED");
		postDataHandler->finish();
		return;
	}
	
	if(!postDataHandler->isSendedSuccesfully()){
		Serial.println("SEND FINISHED WITH NO SUCCESS CODE");
		postDataHandler->finish();
		return;
	}
	
	postDataHandler->finish();
	
	if(!simHandler.disconnectFromGPRS()){
		Serial.println("Failed to close GPRS");
	}
	
	showEntry(temp, press);
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
void askServerData(){
	if(!checkSimModuleReady()){
		Serial.println("ASK FOR DATA FAILED!");
		return;
	}
	
	DataHandler<BUFFER_SIZE>* getHandler = simHandler.sendGetRequest();
	
	if(!getHandler){
		Serial.println("Handler is nullptr");
		return;
	}
	
	getHandler->write("http://128.69.240.186/ReadRaw.php");
	
	if(getHandler->send()){
		while(!getHandler->isSended()){
			delay(1000);
			Serial.println("Send took 1 sec");
		}
	}else{
		Serial.println("SEND FAILED");
		getHandler->finish();
		return;
	}
	
	if(!getHandler->isSendedSuccesfully()){
		Serial.println("SEND FINISHED WITH NO SUCCESS CODE");
		getHandler->finish();
		return;
	}
	
	getHandler->getBuffer().clear();
	while(getHandler->readResponce()){
		auto& b = getHandler->getBuffer();
		int index;
		
		while((index = b.indexOf("\n")) != -1){
			if(index == -1)
				continue;
			
			int startIndex = b.indexOf("Temperature");
			int temp = atof(&b[startIndex + 17]);
			
			startIndex = b.indexOf("Pressure");
			int pressure = atof(&b[startIndex + 15]);
			
			showEntry(temp, pressure);
			
			startIndex = b.indexOf("Time: ");
			if(startIndex != -1){
				Clock tmpClock;
				if(tmpClock.parse(&b[startIndex + 6])){
					printTime(tmpClock);
					delay(1500);
				}
				
			}
			
			b.substring(index);	//substring from /nTemperature of new line
			b.remove(0, 1);	//delete /n symbol
			
		}
		
	}
	
	getHandler->finish();
	
	if(!simHandler.disconnectFromGPRS()){
		Serial.println("Failed to close GPRS");
	}
}


//Check if sim module is working and tries to open GPRS connection
bool checkSimModuleReady(){
	if(!simHandler.isModuleUp()){
		delay(5000);
		if(!simHandler.isModuleUp()){
			Serial.println("Module if offline");
			return false;
		}
	}
	
	if(!simHandler.isModuleAlive()){
		Serial.println("Module is not alive");
			return false;
	}
	
	if(simHandler.isConnectedToNetwork() != 
						NETWORK_CONNECTION::REGISTERED){
		delay(5000);
		if(simHandler.isConnectedToNetwork() != 
						NETWORK_CONNECTION::REGISTERED){
			Serial.println("Module can't connect to network");
			return false;
		}
	}
	
	if(!simHandler.connectToGPRS("internet")){
		delay(5000);
		if(!simHandler.connectToGPRS("internet")){
			Serial.println("Module can't connect to GPRS");
			return false;
		}
	}
	
	return true;
}


//CAUTION max length of str is 16 symbols
void printMessage(const char* str){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(str);
}