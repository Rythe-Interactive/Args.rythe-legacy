#pragma once
#include "Event.h"

namespace Args
{
	namespace Events
	{
		struct Exit : public Event<Exit>
		{
			int exitCode;
			Exit(int exitCode = 0) : exitCode(exitCode) {}
		};
	}
}