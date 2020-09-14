#include <Arduino.h>
#include "SimHandler.h"


bool SimHandler::init(SoftwareSerial* pSerial){
	if(!pSerial)
		return false;
	
	pSimModule = pSerial;
	return true;
}


bool SimHandler::writeCommand(const String& command){
	if(command.equals(previousCommand))
		return false;
	Serial.println("WRITE - " + command);
	pSimModule->println(command);
	previousCommand = command;
	buffer = "";
	resultCode = UNDEFINED;
	return true;
}

bool SimHandler::isAnwserReady(){
	if(!readToBuffer())
		return false;
	
	return stripCode();
}

bool SimHandler::isAnwserCodeEqual(Anwser_codes code){
	return resultCode == code;
}

bool SimHandler::writeCommandAndCheckAnwser(const String& command){
	if(!writeCommand(command)){
		if(isAnwserReady() && isAnwserCodeEqual(OK)){
			// Serial.println("Anwser is ready");
			return true;
		}
	}
	return false;
}

String SimHandler::getResultMessage() const {
	return buffer;
}

int SimHandler::getResultCode() const {
	return resultCode;
}

void SimHandler::clearPreviousBuffer(){
	previousCommand = "";
}


bool SimHandler::readToBuffer(){
	bool read = false;
	while(pSimModule->available()){
		//TODO: made cpp cast, check of reference
		char c = pSimModule->read();
		Serial.print(c);
		buffer += c;
		read = true;
	}
	return read;
}

/**
	If ATV0 then anwser coud look like:
		Information response: 		<text><CR><LF>
		Short result code format: 	<numeric code><CR>
	If ATV1 ->:
		Information response: 		<CR><LF><text><CR><LF>
		Long result code format: 	<CR><LF><verbose code><CR><LF>
		
	<LF> in ASCI and C = 10 = '\n'
	<CR> in ASCI and C = 13 = '\r'
	
	I suppose that we use ATV0
	So, first check last char if it is <CR>
		then trim(), take away last character, 
		trim() buffer
		
	//TODO: - when you written command it may be read half way
		because it require time to transfer data
		so add some sort of delay before first reading
		
		And there is another trap, some commands won't return any code,
		but a message to send data for example
		
	//TODO: not handled case when somthing happens, like sms or call, or network failure
	//TODO: not handled when you send http data and get anwser,
		it looks like AT+HTTPACTION=1	-> OK\r+HTTPACTION:method,status,length of incoming data
	
	@return true if code was taken away
*/

bool SimHandler::stripCode(){
	if(buffer.length() == 0)
		return false;
	// Serial.println("BUFFER - " + buffer);
	//case when send AT+HTTPDATA=length,time
	if(buffer.indexOf("DOWNLOAD") != -1){
		resultCode = OK;
		return true;
	}
	if(buffer.startsWith("+HTTPACTION:")){
		if(!buffer.endsWith("\n"))
			return false;
		resultCode = OK;
		return true;
	}
	
	if(buffer[buffer.length() - 1] == '\n'){
		buffer.trim();
		
		char last = buffer[buffer.length() - 1];
		// Serial.println("LAST - " + last);
		
		//if last digit is not a code, maybe anwser is not full
		if(!isdigit(last)){
			return false;
		}
		if(buffer.length() > 1 &&
			!isSpace(buffer[buffer.length() - 2])){
			return false;
		}
		resultCode = String(last).toInt();
		// Serial.print("");
		// Serial.print(buffer);
		// Serial.print('\n');
		//You can get only code message from some commands
		if(buffer.length() > 0){
			// Serial.print(buffer);
			buffer.remove(buffer.length() - 1, 1);
			// buffer = buffer.substring(0, buffer.length());
			buffer.trim();
			// Serial.print("-");
			// Serial.print(buffer);
			// Serial.println();
			
		}
		return true;
	}
	return false;
}