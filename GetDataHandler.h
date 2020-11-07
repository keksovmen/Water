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
		GetDataHandler(	SimTools& tools,
						FixedBufferBase& buffer
						);
		
		bool send() override;
		
};