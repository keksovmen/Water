#pragma once


#include "SimCommandWriter.h"
#include "BaseReader.h"


class SimCommandPort : public SimCommandWriter, public BaseReader
{
	public:
		SimCommandPort(CommandWriter& writer, BaseReader& reader);
		
		//delegate to refReader
		bool read() override;
		bool readTimeout(unsigned long maxDelay) override;
		
		
	protected:
		BaseReader& refReader;
		
		
		
	
};
