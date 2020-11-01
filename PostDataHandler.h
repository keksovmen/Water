#pragma once

#include "DataHandler.h"


/**
	Child of DataHandler
	Handles POST messages
	
	Write commands will write into POST body
*/

class PostDataHandler : public DataHandler
{
	public:
		PostDataHandler(	SimResultParser& parser, 
							SimCommandPort& simPort,
							FixedBufferBase& buffer,
							SimState& state
							);

		bool send() override;
		
};