#pragma once
#include "Enums.h"
#include "LongCommandHandler.h"
#include "TimeHandler.h"


struct SimState
{
	struct StateHealth{
		bool defaultsAreSet = false;
		bool cpin = false;
		bool networkRegistration = false;
		bool CGATT_Connection = false;
		BEARER_STATUS GPRS_Connection = GPRS_UNDEFINIED;
		bool isImeiSet = false;
	} health;
	
	
	struct StateHTTP{
		bool isBussy = false;
		bool isAnwserReady = false;
		long responseLength = 0;
		int responseCode = 0;
	} http;
	
	
	struct StateTCP{
		TCP_STATE state = TCP_STATE_UNDEFINIED;
		bool hasMessage = false;
		bool isSending = false;
		bool hasToSendPong = false;
		bool hastToSendId = true;
		bool hasToSendAcknowledgment = false;
	} tcp;
	
	
	struct StateLongCommand{
		LongCommandHandler* cmdHandler = nullptr;
		bool isAnwserReady = false;
	} longCmd;
	
	

	TimeHandler timer;
	
	
	
	/*-----------------FUNCTIONS--------------------*/
	
	bool isMinimumEstablished(){
		return health.cpin && 
			health.networkRegistration;
	}
	
	
	void setLongCmd(LongCommandHandler* cmd = nullptr){
		longCmd.cmdHandler = cmd;
		longCmd.isAnwserReady = false;
	}
	
	
	bool isGPRS_Connected(){
		return health.GPRS_Connection == GPRS_CONNECTED;
	}
	
	
	void clearHTTP(){
		http = StateHTTP();
	}
	
	
	bool isTCP_Connected(){
		return tcp.state == TCP_STATE_CONNECTED;
	}
	
	
	void diedGPRS(){
		health.GPRS_Connection = GPRS_UNDEFINIED;
		//TODO: prevent http request from waiting for eternity
	}
	
	
	void diedCGATT(){
		tcp.state = TCP_STATE_PDP_DEACT;
		tcp.isSending = false;
		tcp.hasToSendPong = false;
		tcp.hastToSendId = true;
		tcp.hasToSendAcknowledgment = false;
		
		health.CGATT_Connection = false;
	}
	
	
	void encounterUndefinied(){
		health.cpin = false;
		health.networkRegistration = false;
		health.GPRS_Connection = GPRS_UNDEFINIED;
		health.CGATT_Connection = false;
		
		tcp.state = TCP_STATE_UNDEFINIED;
		
	}
	
	
	void setDefault(){
		health = StateHealth();
		http = StateHTTP();
		tcp = StateTCP();
		longCmd = StateLongCommand();
	}
	
	
	void readyDetected(){
		setDefault();
		timer.sheduleDelay(7000);
	}
	
	
	bool isAbleToSendHttp(){
		return !http.isBussy && 
				longCmd.cmdHandler == nullptr &&
				isMinimumEstablished() &&
				isGPRS_Connected();
	}
};