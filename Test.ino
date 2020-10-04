#include <LiquidCrystal_I2C.h>
#include "SimHandler.h"
#include "Clock.h"
#include "Device.h"


#define BUFFER_SIZE 128
#define BUTTON_TIME 8
#define BUTTON_SEND 9
#define BUTTON_SHOW 10

Device dev;

SoftwareSerial sim (2, 3);
SimHandler<BUFFER_SIZE> simHandler(sim);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Clock clk;


// void askForTime();

unsigned long timeBefore;
// unsigned long timePassed = 15000;
// bool sended = false;


void setup(){
	Serial.begin(9600);
	sim.begin(9600);
	
	pinMode(BUTTON_TIME, INPUT);
	pinMode(BUTTON_SEND, INPUT);
	pinMode(BUTTON_SHOW, INPUT);
	
	digitalWrite(BUTTON_TIME, HIGH);
	digitalWrite(BUTTON_SEND, HIGH);
	digitalWrite(BUTTON_SHOW, HIGH);
	
	lcd.init();
	lcd.backlight();
	
	if(!dev.init()){
		Serial.println("Device is not connected");
		while(1){};
	}
	
	if(!simHandler.isModuleUp()){
		delay(5000);
		if(!simHandler.isModuleUp()){
			Serial.println("Module if offline");
			while(1){};
		}
	}
	
	if(!simHandler.isModuleAlive()){
		Serial.println("Module is not alive");
		while(1){};
	}
	
	if(!simHandler.setDefaultParams()){
		Serial.println("Defaults are not set");
		while(1){};
	}
	
	timeBefore = millis();
	// askForTime();
	// sendSensorData();
}

void loop(){
	unsigned long timeNow = millis();
	int timePassed = timeNow - timeBefore;
	timeBefore = timeNow;
	
	if(clk.addMillis(timePassed)){
		printTime();
	}
	
	if(digitalRead(BUTTON_TIME) == LOW){
		printAskintTime();
		askForTime();
	}
	// delay(1000);
	// if(Serial.available()){
		// sim.write(Serial.read());
	// }
	// if(sim.available()){	
		// Serial.write(sim.read());
	// }
	
}


void askForTime(){
	if(!checkSimModuleReady()){
		Serial.println("ASK FOR TIME FAILED!");
		return;
	}
	
	GetDataHandler<BUFFER_SIZE> getHandler = simHandler.sendGetRequest();
	
	getHandler.writeString("http://128.69.240.186/GetTime.php");
	
	if(getHandler.send()){
		if(!getHandler.isSended()){
			if(!getHandler.isSended()){
				Serial.println("Send already took 10 sec");
				while(!getHandler.isSended()){
					Serial.println("Send took another 5 sec");
				}
			}
		}
	}else{
		Serial.println("SEND FAILED");
		getHandler.finish();
		return;
	}
	
	if(!getHandler.isSendedSuccesfully()){
		Serial.println("SEND FINISHED WITH NO SUCCESS CODE");
		getHandler.finish();
		return;
	}
	
	getHandler.getBuffer().clear();
	if(getHandler.readResponce()){
		auto& b = getHandler.getBuffer();
		if(!clk.parse(b.begin())){
			Serial.println("CLOCK PARSE FAILED MUST BE DATA CORRAPTION HERE IS BUFFER:");
			Serial.println(b.begin());
			getHandler.finish();
			return;
		}
	}
	
	getHandler.finish();
	
	if(!simHandler.disconnectFromGPRS()){
		Serial.println("Failed to close GPRS");
	}
}


void printTime(){
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

void sendSensorData(){
	if(!checkSimModuleReady()){
		Serial.println("SEND SENSOR DATA FAILED");
		return;
	}
	
	
	dev.readResults();
	int temp = dev.getTemperature();
	int press = dev.getPressure();
	// const char* str = "temperature=69&pressure=1000";
	//TODO: made some class to monitor length
	PostDataHandler<BUFFER_SIZE> postDataHandler = simHandler.sendPostRequest("http://128.69.240.186/Send.php", 28);
	postDataHandler.writeString("temperature=");
	postDataHandler.writeInt(temp);
	postDataHandler.writeString("&pressure=");
	postDataHandler.writeInt(press);
	
	// postDataHandler.writeString(str);
	
	if(postDataHandler.send()){
		if(!postDataHandler.isSended()){
			if(!postDataHandler.isSended()){
				Serial.println("Send already took 10 sec");
				while(!postDataHandler.isSended()){
					Serial.println("Send took another 5 sec");
				}
			}
		}
	}else{
		Serial.println("SEND FAILED");
		postDataHandler.finish();
		return;
	}
	
	if(!postDataHandler.isSendedSuccesfully()){
		Serial.println("SEND FINISHED WITH NO SUCCESS CODE");
		postDataHandler.finish();
		return;
	}
	
	postDataHandler.finish();
	
	if(!simHandler.disconnectFromGPRS()){
		Serial.println("Failed to close GPRS");
	}
	
	showEntry(temp, press);
}

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
	
	if(!simHandler.isConnectedToNetwork()){
		delay(5000);
		if(!simHandler.isConnectedToNetwork()){
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

void printAskintTime(){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Asking time");
}