#pragma once
#include "Utils/Common.h"

namespace Args
{
	struct IEvent
	{
	protected:
		static uint32 eventCount;

		virtual uint32 GetTypeID() = 0;
	};

	template<typename Self>
	struct Event : public IEvent
	{
		static uint32 id;

		virtual uint32 GetTypeID()
		{
			return id;
		}
	};

	template<typename Self>
	uint32 Event<Self>::id = eventCount++;
}