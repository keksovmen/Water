#pragma once
#include "Enums.h"
#include "LongCommandHandler.h"


struct SimState
{
	struct StateHealth{
		bool defaultsAreSet = false;
		bool cpin = false;
		bool networkRegistration = false;
		BEARER_STATUS GPRS_Connection = GPRS_UNDEFINIED;
		bool CGATT_Connection = false;
	} health;
	
	
	struct StateHTTP{
		bool isBussy = false;
		bool isAnwserReady = false;
		long responseLength = 0;
	} http;
	
	
	struct StateTCP{
		TCP_STATE state = TCP_STATE_WRITING_DEFAULTS;
		bool hasMessage = false;
		bool isSending = false;
	} tcp;
	
	
	struct StateLongCommand{
		LongCommandHandler* cmdHandler = nullptr;
		bool isAnwserReady = false;
	} longCmd;
	
	
	bool isReadyEncountered = false;
	unsigned long readyTimeEncountered = 0;
	
	
	
	bool isMinimumEstablished(){
		return health.cpin && health.networkRegistration;
	}
	
	
	void setLongCmd(LongCommandHandler* cmd = nullptr){
		longCmd.cmdHandler = cmd;
		longCmd.isAnwserReady = false;
	}
	
	
	bool isGPRS_Connected(){
		return health.GPRS_Connection == GPRS_CONNECTED;
	}
};