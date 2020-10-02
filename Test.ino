// #include "Device.h"
// #include "GPRS.h"
// #include "HTTP.h"
// #include "SimHandler.h"
// #include "SimIOWrapper.h"
// #include "SimResultParser.h"
// #include "SimCommandWriter.h"
#include "SimHandler.h"
// #include "Buffer/FixedBuffer.h"


#define BUFFER_SIZE 128

// Device dev;
SoftwareSerial sim (2, 3);
// HTTP http;
// SimHandler simHandler;
// SimIOWrapper<BUFFER_SIZE> wrapper (sim);
// SimResultParser<BUFFER_SIZE> parser;
// SimCommandWriter<BUFFER_SIZE> writer;
SimHandler<BUFFER_SIZE> simHandler(sim);

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
			return;
		}
	}
	
	if(!simHandler.setDefaultParams()){
		Serial.println("Defaults are not set");
		return;
	}
	
	if(!simHandler.isConnectedToNetwork()){
		delay(5000);
		if(!simHandler.isConnectedToNetwork()){
			Serial.println("Module can't connect to network");
			return;
		}
	}
	
	if(!simHandler.connectToGPRS("internet")){
		delay(5000);
		if(!simHandler.connectToGPRS("internet")){
			Serial.println("Module can't connect to GPRS");
			return;
		}
	}
	
	const char* str = "temperature=69&pressure=1000";
	const int strLength = strlen(str);
	
	PostDataHandler<BUFFER_SIZE> postDataHandler = simHandler.sendPostRequest("http://128.69.240.186/Send.php", strLength);
	postDataHandler.writeString(str);
	
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
		Serial.println("Module can't go furthre then HTTPACTION");
		return;
	}
	
	if(!postDataHandler.isSendedSuccesfully()){
		Serial.println("Send finished with not Success code");
	}
	
	postDataHandler.finish();
	
	
	GetDataHandler<BUFFER_SIZE> getHandler = simHandler.sendGetRequest();
	
	getHandler.writeString("http://128.69.240.186/ReadRaw.php");
	
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
		Serial.println("Module can't go furthre then HTTPACTION");
		return;
	}
	
	getHandler.getBuffer().clear();
	int counter = 0;
	while(getHandler.readResponce()){
		auto& b = getHandler.getBuffer();
		int index = b.indexOf("\n");
		index++;
		if(index == -1){
			continue;
		}
		
		counter++;
		for(int i = 0; i < index; i++){
			Serial.print(b[i]);
		}

		b.substring(index);
	}
	
	Serial.print("COUNTER = ");
	Serial.println(counter);
	
	
	getHandler.finish();
	
	simHandler.disconnectFromGPRS();
	
	
	// Serial.println(simHandler.setDefaultParams() ? "TRUE" : "FALSE");
	// Serial.println(simHandler.connectToGPRS("\"internet\"") ? "TRUE" : "FALSE");
	// Serial.println(simHandler.disconnectFromGPRS() ? "TRUE" : "FALSE");
	
	// Serial.println(simHandler.isModuleUp() ? "TRUE" : "FALSE");
	// Serial.println(simHandler.isConnectedToNetwork());
	
	// writer.writeAT();
	// wrapper.readToBuffer();
	// Serial.println(parser.isSimpleMessageReady() ? "TRUE" : "FALSE");
	// writer.writeCREG();
	// wrapper.readToBuffer();
	// Serial.println(parser.isComplexMessageReady() ? "TRUE" : "FALSE");
	// writer.writeCSQ();
	// wrapper.readToBuffer();
	// Serial.println(parser.isComplexMessageReady() ? "TRUE" : "FALSE");
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
