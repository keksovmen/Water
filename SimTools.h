#pragma once

#include "SimCommandPort.h"
#include "SimResultParser.h"
#include "SimState.h"



class SimTools
{
	public:
		SimCommandPort simPort;
		SimResultParser parser;
		SimState state;
	
	
		SimTools(
				CommandWriter& writer, 
				BaseReader& reader,
				FixedBufferBase& buffer
				) :
				simPort(writer, reader), parser(buffer){}
	
		/**
			Tries to read from buffer
			and parse result anwser
			
			@return ANWSER_CODES if success or mine UNDEFINED if failed
		
		*/

		virtual ANWSER_CODES readAndGetCode();
		
		
		/**
			Tries to read from buffer
			and parse result anwser
			
			@return true if result is present and equal OK
		
		*/

		virtual bool readAndExpectSuccess();
		
	
	
};


class SimToolsProxy : public SimTools
{
	public:
		SimToolsProxy(
				CommandWriter& writer, 
				BaseReader& reader,
				FixedBufferBase& buffer
				) :
				SimTools(writer, reader, buffer){}
				
				
		virtual ANWSER_CODES readAndGetCode() override;
				
	
};