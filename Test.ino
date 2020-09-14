#include "Device.h"
#include "GPRS.h"
#include "HTTP.h"
#include "SimHandler.h"

Device dev;
SoftwareSerial sim (2, 3);
// GPRS gprs;
HTTP http;
SimHandler simHandler;

//Excpected sim module flags ATE0 and ATV0

unsigned long timeBefore;
unsigned long timePassed = 15000;
bool sended = false;


void setup(){
	Serial.begin(9600);
	sim.begin(9600);
	// gprs.init(&sim);
	simHandler.init(&sim);
	http.init(&simHandler);
	while(!Serial){
	}
	// sim.println("AT");
	// delay (50);
	// while(sim.available() > 0){
		// Serial.write(sim.read());
	// }
	// if(!dev.init()){
		// Serial.println("ERROR WITH VALIDITY");
		// while(1){}
	// }
	Serial.println("CONNECTED");
	// dev.doActions(10000);
}

void loop(){
	unsigned long timeAfter = millis();
	unsigned long deltaTime = timeAfter - timeBefore;
	timeBefore = timeAfter;
	timePassed += deltaTime;
	if(timePassed > 15000){
		if(sended){
			int state = http.doActions(deltaTime);
			// Serial.println(state);
			if(state == HTTP_state::idle){
				sended = false;
				timePassed = 0;
			}
		}else{
			http.setDataToSend("temperature=69&pressure=420");
			sended = true;
		}
	}
	
	// gprs.doActions(deltaTime);
	// Serial.println(gprs.doActions(deltaTime));
	
	// if(gprs.buffer.length() > 0){
		// Serial.println(gprs.buffer);
	// }
	
	// if(Serial.available()){
		// sim.write(Serial.read());
	// }
	// if(sim.available()){	
		// Serial.write(sim.read());
	// }
	
	
	
	// Serial.print("Temperature - C: ");
	// Serial.print(dev.getTemperature(), 2);
	// Serial.println("");
	// Serial.print("Pressure - mb: ");
	// Serial.print(dev.getPressure(), 2);
	// Serial.println("");
}
