#pragma once
#include <chrono>
#include "Time.h"

namespace Args
{
	class Clock
	{
	private:
		std::chrono::time_point<std::chrono::system_clock> start;
	public:
		void Start();
		Time GetElapsedTime() const;
		Time End() const;
		Time Restart();
	};
}
