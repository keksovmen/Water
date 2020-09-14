#pragma once
#include <SoftwareSerial.h>

/**
	For internet transmition usage
	only OK, ERROR codes are available.
	Other codes for calls.
	
	All codes and their description on page 43
*/

enum Anwser_codes : int
{
	OK = 0,	//Command handled correctly
	CONNECT = 1,	/*	A connection has been established; the DCE is moving from
						Command state to online data state	*/
	RING = 2, //The DCE has detected an incoming call signal from network
	NO_CARRIER = 3, /*	The connection has been terminated or the attempt to
						establish a connection failed	*/
	ERROR = 4,	/*	Command not recognized, Command line maximum length
					exceeded, parameter value invalid, or other problem with
					processing the Command line	*/
	NO_DIALTONE = 6, //No dial tone detected
	BUSY = 7,	//Engaged (busy) signal detected
	NO_ANSWER = 8,	/*	"@" (Wait for Quiet Answer) dial modifier was used, but
						remote ringing followed by five seconds of silence was not
						detected before expiration of the connection timer (S7)	*/
	PROCEEDING = 9,	//An AT command is being processed
	UNDEFINED = 10	//my type for code to work
	
};

/**
	How to use:
		1 - init();
		2 - writeCommand("cmd") - call repetedly, and handle result
		3 - isAnwserReady() - if so go to 4, if not call repetedly
		4 - getResultCode() - check if it is OK or ERROR, if OK go to 5
		5 - getResultMessage() - if there is anything interesting
*/

class SimHandler
{
	public:
		bool init(SoftwareSerial* pSerial);
		//return true if previous command is not the same
		//return false if previous command is the same
		bool writeCommand(const String& command);
		
		//return true if anwser has come to buffer
		bool isAnwserReady();
		
		//compare @resultCode
		bool isAnwserCodeEqual(Anwser_codes code);
		
		//short cut for, writing and then checking if result is OK
		bool writeCommandAndCheckAnwser(const String& command);
		
		//Just copy underlying buffer away
		String getResultMessage() const;
		
		int getResultCode() const;
		
		//When you want to try to repeat last command
		void clearPreviousBuffer();
		
	private:
		//return true if read something, false otherwise
		bool readToBuffer();
		
		//return true if @Anwser_codes was taken away from buffer,
		//and set to @resultCode
		bool stripCode();
		
	
		//must be initiated
		SoftwareSerial* pSimModule;
	
		//will collect result
		String buffer;
		
		//compare against this when writing command
		String previousCommand;
		
		//Will be parsed from @buffer String
		int resultCode;
	
};