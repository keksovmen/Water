#include "SimCommandWriter.h"


SimCommandWriter::SimCommandWriter(CommandWriter& writer) :
	refWriter (writer){
	
}


void SimCommandWriter::write(const char* str){
	refWriter.write(str);
}


void SimCommandWriter::write(char c){
	refWriter.write(c);
}


void SimCommandWriter::write(int i){
	refWriter.write(i);
}


void SimCommandWriter::write(long l){
	refWriter.write(l);
}


void SimCommandWriter::write(double d, int amountAfterDot){
	refWriter.write(d, amountAfterDot);
}


void SimCommandWriter::writeEndOfCommand(bool clearBuffer){
	refWriter.writeEndOfCommand(clearBuffer);
}


void SimCommandWriter::writeAT(){
	refWriter.writeCommand("AT");
}


void SimCommandWriter::writeCREG(){
	refWriter.writeCommand("AT+CREG?");
}


void SimCommandWriter::writeCSQ(){
	refWriter.writeCommand("AT+CSQ");
}


void SimCommandWriter::writeEcho(bool turnOn){
	refWriter.write("ATE");
	refWriter.write(turnOn ? '1' : '0');
	refWriter.write("&W");
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeNumberFormat(bool turnOn){
	refWriter.write("ATV");
	refWriter.write(turnOn ? '0' : '1');
	refWriter.write("&W");
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeCallReady(bool turnOn){
	refWriter.write("AT+CIURC=");
	refWriter.write(turnOn ? '1' : '0');
	refWriter.write(";&W");
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeReportAsError(bool turnOn){
	refWriter.write("AT+CMEE=");
	refWriter.write(turnOn ? '1' : '0');
	refWriter.write(";&W");
	refWriter.writeEndOfCommand();
}



void SimCommandWriter::writeSAPBR(SAPBR_COMMANDS cmd, const char* param, const char* value){
	refWriter.write("AT+SAPBR=");
	refWriter.write(static_cast<int>(cmd));
	refWriter.write(",1");
		
	if((param) && (value)){
		refWriter.write(",\"");
		refWriter.write(param);
		refWriter.write("\",\"");
		refWriter.write(value);
		refWriter.write('"');
	}
	
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeHTPP(HTTP_COMMANDS cmd){
	switch (cmd){
		case HTTP_INIT:
			refWriter.write("AT+HTTP");	//memory saving
			refWriter.writeCommand("INIT");
		break;
		
		case HTTP_TERM:
			refWriter.write("AT+HTTP");	//memory saving
			refWriter.writeCommand("TERM");
		break;
			
		default: break;
	}
}


void SimCommandWriter::writeHTTPURL(
					IPAddress& address, 
					const char* url,
					bool endCommand
					){
	refWriter.write("AT+HTTP");	//memory saving
	refWriter.write("PARA=");
	refWriter.write('"');
	refWriter.write("URL");
	refWriter.write("\",\"http://");
	refWriter.write(address[0]);
	refWriter.write('.');
	refWriter.write(address[1]);
	refWriter.write('.');
	refWriter.write(address[2]);
	refWriter.write('.');
	refWriter.write(address[3]);
	refWriter.write('/');
	refWriter.write(url);
	//for get post request
	if(endCommand){
		refWriter.write('"');
		refWriter.writeEndOfCommand();
	}
}


void SimCommandWriter::writeHTPPSetParam(const char* param, const char* value){
	//TODO: made separate header file with all comands as fields
	refWriter.write("AT+HTTP");	//memory saving
	refWriter.write("PARA=");
	refWriter.write('"');
	refWriter.write(param);
	refWriter.write("\",\"");
		
	if(value){
		refWriter.write(value);
		refWriter.write('"');
		refWriter.writeEndOfCommand();
	}
	//else will be written later through other methods of SimIOWrapper
}



void SimCommandWriter::writeHTPPAction(HTTP_REQUESTS method){
	refWriter.write("AT+HTTP");	//memory saving
	refWriter.write("ACTION=");
	refWriter.write(static_cast<int>(method));
	refWriter.writeEndOfCommand();
	
}


void SimCommandWriter::writeHTPPData(int length){
	refWriter.write("AT+HTTP");	//memory saving
	refWriter.write("DATA=");
	refWriter.write(length);
	refWriter.write(',');
	refWriter.write(5000);
	refWriter.writeEndOfCommand();
}



void SimCommandWriter::writeReadHTTP(int from, int amount){
	refWriter.write("AT+HTTP");	//memory saving
	refWriter.write("READ=");
	refWriter.write(from);
	refWriter.write(',');
	refWriter.write(amount);
	refWriter.writeEndOfCommand(false);
}



void SimCommandWriter::writeIPR(long rate){
	refWriter.write("AT+IPR=");
	refWriter.write(rate);
	refWriter.write(";&W");
	refWriter.writeEndOfCommand();
}



void SimCommandWriter::writeCPIN(){
	refWriter.writeCommand("AT+CPIN?");
}



void SimCommandWriter::writeDenyCall(){
	refWriter.writeCommand("ATH", false);
}


void SimCommandWriter::writeCIPRXGET(CIPRXGET_COMMAND cmd, int arg){
	refWriter.write("AT+CIPRXGET");
	
	if(cmd == CIPRXGET_COMMAND::CIPRXGET_COMMAND_MODE){
		refWriter.write('?');
		refWriter.writeEndOfCommand();
		
		return;
	}
	
	refWriter.write('=');
	refWriter.write(cmd);
	
	switch(cmd){
		case CIPRXGET_COMMAND_GET_NORMAL :
		case CIPRXGET_COMMAND_GET_HEX :
			refWriter.write(',');
			refWriter.write(arg);
			break;
			
		default : break;
	}
	
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeCGATT(CGATT_COMMANDS cmd){
	refWriter.write("AT+CGATT");
	switch(cmd){
		case CGATT_COMMANDS_STATUS:
			refWriter.write('?');
			refWriter.writeEndOfCommand();
			return;
		
		case CGATT_COMMANDS_ON:
			refWriter.write('=');
			refWriter.write(1);
			break;
			
		case CGATT_COMMANDS_OFF:
			refWriter.write('=');
			refWriter.write(0);
			break;
	}
	
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeCSTT(const char* apn){
	refWriter.write("AT+CSTT=\"");
	refWriter.write(apn);
	refWriter.write('"');
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeCIICR(){
	refWriter.writeCommand("AT+CIICR");
}


void SimCommandWriter::writeCIPSTATUS(){
	refWriter.writeCommand("AT+CIPSTATUS");
}


void SimCommandWriter::writeCIPSTART(IPAddress& address, long port){
	refWriter.write("AT+CIPSTART=");
	refWriter.write('"');
	refWriter.write("TCP");
	refWriter.write('"');
	refWriter.write(',');
	refWriter.write('"');
	refWriter.write(address[0]);
	refWriter.write('.');
	refWriter.write(address[1]);
	refWriter.write('.');
	refWriter.write(address[2]);
	refWriter.write('.');
	refWriter.write(address[3]);
	refWriter.write('"');
	refWriter.write(',');
	refWriter.write('"');
	refWriter.write(port);
	refWriter.write('"');
	refWriter.writeEndOfCommand();
}


void SimCommandWriter::writeGetIpTCP(){
	refWriter.writeCommand("AT+CIFSR");
}


void SimCommandWriter::writeCIPSHUT(){
	refWriter.writeCommand("AT+CIPSHUT");
}


void SimCommandWriter::writeCIPSEND(){
	refWriter.writeCommand("AT+CIPSEND");
}