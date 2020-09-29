// #include "Device.h"
// #include "GPRS.h"
// #include "HTTP.h"
// #include "SimHandler.h"
// #include "SimIOWrapper.h"
// #include "SimResultParser.h"
// #include "SimCommandWriter.h"
#include "SimFacade.h"
// #include "Buffer/FixedBuffer.h"


#define BUFFER_SIZE 128

// Device dev;
SoftwareSerial sim (2, 3);
// HTTP http;
// SimHandler simHandler;
// SimIOWrapper<BUFFER_SIZE> wrapper (sim);
// SimResultParser<BUFFER_SIZE> parser;
// SimCommandWriter<BUFFER_SIZE> writer;
SimFacade<BUFFER_SIZE> simHandler(sim);

//Excpected sim module flags ATE0 and ATV0

// unsigned long timeBefore;
// unsigned long timePassed = 15000;
// bool sended = false;


void setup(){
	Serial.begin(9600);
	sim.begin(9600);
	if(!simHandler.isModuleUp()){
		delay(5000);
		if(!simHandler.isModuleUp()){
			Serial.println("Module if offline");
			while(1){}
		}
	}
	
	// Serial.println(simHandler.setDefaultParams() ? "TRUE" : "FALSE");
	Serial.println(simHandler.connectToGPRS("\"internet\"") ? "TRUE" : "FALSE");
	Serial.println(simHandler.disconnectFromGPRS() ? "TRUE" : "FALSE");
	
	// Serial.println(simHandler.isModuleUp() ? "TRUE" : "FALSE");
	// Serial.println(simHandler.isConnectedToNetwork());
	
	// writer.writeAT(wrapper);
	// wrapper.readToBuffer();
	// Serial.println(parser.isSimpleMessageReady(wrapper.getBuffer()) ? "TRUE" : "FALSE");
	// writer.writeCREG(wrapper);
	// wrapper.readToBuffer();
	// Serial.println(parser.isComplexMessageReady(wrapper.getBuffer()) ? "TRUE" : "FALSE");
	// writer.writeCSQ(wrapper);
	// wrapper.readToBuffer();
	// Serial.println(parser.isComplexMessageReady(wrapper.getBuffer()) ? "TRUE" : "FALSE");
	// gprs.init(&sim);
	// simHandler.init(&sim);
	// http.init(&simHandler);
	// while(!Serial){
	// }
	// sim.println("AT");
	// delay (50);
	// while(sim.available() > 0){
		// Serial.write(sim.read());
	// }
	// if(!dev.init()){
		// Serial.println("ERROR WITH VALIDITY");
		// while(1){}
	// }
	// Serial.println("CONNECTED");
	// dev.doActions(10000);
}

void loop(){
	// wrapper.writeCommand("AT");
	// unsigned long timeAfter = millis();
	// unsigned long deltaTime = timeAfter - timeBefore;
	// timeBefore = timeAfter;
	// timePassed += deltaTime;
	// if(dev.doActions(deltaTime)){
		// timeAfter = millis();
		// deltaTime = timeAfter - timeBefore;
		// timePassed += deltaTime;
		// timeBefore = timeAfter;
	// }
	
	// if(timePassed > 30000){
		// if(sended){
			// int state = http.doActions(deltaTime);
			// Serial.println(state);
			// if(state == HTTP_state::idle){
				// sended = false;
				// timePassed = 0;
			// }
		// }else{
			
			// http.setDataToSend("temperature=" + 
			// String(dev.getTemperature(), 2) + 
			// "&pressure=" + 
			// String(dev.getPressure(), 3));
			// sended = true;
		// }
	// }
	
	// gprs.doActions(deltaTime);
	// Serial.println(gprs.doActions(deltaTime));
	
	// if(gprs.buffer.length() > 0){
		// Serial.println(gprs.buffer);
	// }
	
	if(Serial.available()){
		sim.write(Serial.read());
	}
	if(sim.available()){	
		Serial.write(sim.read());
	}
	
	
	
	// Serial.print("Temperature - C: ");
	// Serial.print(dev.getTemperature(), 2);
	// Serial.println("");
	// Serial.print("Pressure - mb: ");
	// Serial.print(dev.getPressure(), 2);
	// Serial.println("");
}
