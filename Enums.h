#pragma once

/**
	For internet transmition usage
	only OK, ERROR codes are available.
	Other codes for calls.
	
	All codes and their description on page 43
*/

enum ANWSER_CODES : int
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
	All possible codes from AT+CREG?
	
	0 - Not registered, MT is not currently searching a new
		operator to register to
	1 - Registered, home network
	2 - Not registered, but MT is currently searching a new
		operator to register to
	3 - Registration denied
	4 - Unknown
	5 - Registered, roaming
*/

enum NETWORK_CONNECTION : int
{
	NOT_REGISTERED_NOT_SEARCING = 0,
	REGISTERED = 1,
	NOT_REGISTERED_SEARCHING = 2,
	DENIED = 3,
	UNKNOWN = 4,
	REGISTERED_ROAMING = 5
};