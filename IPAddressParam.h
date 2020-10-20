#pragma once

#include <IPAddress.h>
#include "BaseParameter.h"


class IPAddressParam : public IPAddress, public BaseParameter
{
	public:
		int getLength() override;
		void handleWritingValue(BaseWriter& writer) override;
};
