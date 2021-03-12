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


/**
	All possible methods for AT+SAPBR
	
	0 - Close bearer
	1 - Open bearer
	2 - Query bearer
	3 - Set bearer parameters
	4 - Get bearer parameters
*/

enum SAPBR_COMMANDS : int 
{
	CLOSE_BEARER = 0,
	OPEN_BEARER = 1,
	QUERY_BEARER = 2,
	SET_PARAM_BEARER = 3,
	GET_PARAM_BEARER = 4
};


/**
	All possible values from AT+SAPBR=2,id
		0 - connecting
		1 - connected
		2 - closing
		3 - closed
		4 - undefinied my type for implementation purposes
*/

enum BEARER_STATUS : int
{
	GPRS_CONNECTING = 0,
	GPRS_CONNECTED = 1,
	GPRS_CLOSING = 2,
	GPRS_CLOSED = 3,
	GPRS_UNDEFINIED = 4
};



enum HTTP_COMMANDS : int
{
	HTTP_INIT,
	HTTP_SET_PARAM,
	HTTP_ACTION,
	HTTP_TERM
};


/**
	For AT+HTTPACTION=<n>
		<n>:
			0 - GET
			1 - POST
			2 - HEAD
			3 - DELETE
*/

enum HTTP_REQUESTS : int
{
	HTTP_GET = 0,
	HTTP_POST = 1,
	HTTP_HEAD = 2,
	HTTP_DELETE = 3
};


/**
	Returned in AT+HTTPACTION
	There are only first digits of codes,
	they have 3 digit in each for more info
*/

enum HTTP_STATUS_CODES : int
{
	HTTP_STATUS_INFO = 1,
	HTTP_STATUS_SUCCESS = 2,
	HTTP_STATUS_REDIRECTION = 3,
	HTTP_STATUS_CLIENT_ERROR = 4,
	HTTP_STATUS_SERVER_ERROR = 5,
	HTTP_STATUS_SIM_MODULE_ERROR = 6,
	
};


/**
	Used by Parameter.h
*/

enum PARAMETER_TYPES : int
{
	PARAMETER_INT,
	PARAMETER_DOUBLE,
	PARAMETER_USER_DEFINED
	
};


enum PARSER_STATE
{
	PARSER_STATE_TEXT,
	PARSER_STATE_DIGIT,
	PARSER_STATE_BOTH
};


enum CME_ERROR : int 
{
	CME_ERROR_UNKNOWN = 100,
	CME_ERROR_OPERATION_NOT_ALLOWED = 3,
	CME_ERROR_SIM_BUSSY = 14
};


enum TCP_STATE : int
{
	TCP_STATE_INITIAL,
	TCP_STATE_IP_START,
	TCP_STATE_IP_CONFIG,
	TCP_STATE_IP_GPRS_ACT,
	TCP_STATE_IP_STATUS,
	TCP_STATE_CONNECTING,
	TCP_STATE_CONNECTED,
	TCP_STATE_CLOSING,
	TCP_STATE_CLOSED,
	TCP_STATE_PDP_DEACT,
	TCP_STATE_UNDEFINIED	//for debug purposes
};


enum CIPRXGET_COMMAND : int
{
	CIPRXGET_COMMAND_OFF = 0,
	CIPRXGET_COMMAND_ON = 1,
	CIPRXGET_COMMAND_GET_NORMAL = 2,
	CIPRXGET_COMMAND_GET_HEX = 3,
	CIPRXGET_COMMAND_INFO = 4,
	CIPRXGET_COMMAND_MODE
	
};


enum READ_TYPE
{
	READ_TYPE_HTTP,
	READ_TYPE_TCP
};


enum CGATT_COMMANDS
{
	CGATT_COMMANDS_STATUS,
	CGATT_COMMANDS_ON,
	CGATT_COMMANDS_OFF	
};


enum HTTP_SCRIPT
{
	HTTP_SCRIPT_GET_TIME,
	HTTP_SCRIPT_GET_VOLUME,
	HTTP_SCRIPT_SEND_PARAMS,
	HTTP_SCRIPT_SEND_VOLUME,
	HTTP_SCRIPT_NAN
};


enum PARAMETER_PARSER_RESPONCES
{
	PARAMETER_PARSER_NEED_MORE_DATA,
	PARAMETER_PARSER_NO_MORE_ENTRIES,
	PARAMETER_PARSER_ENTRY_PARSED,
	PARAMETER_PARSER_PING_FOUND
	
};