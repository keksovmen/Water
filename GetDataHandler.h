#pragma once

#include "DataHandler.h"


/**
	Child of DataHandler
	Handles POST messages
	
	Write commands will write into URL body
*/

class GetDataHandler : public DataHandler
{
	public:
		GetDataHandler(	SimResultParser& parser, 
						SimCommandPort& simPort,
						FixedBufferBase& buffer,
						SimState& state
						);
		
		bool send() override;
		
};