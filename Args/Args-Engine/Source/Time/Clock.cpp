#include "Time/Clock.h"

void Args::Clock::Start()
{
	start = std::chrono::system_clock::now();
}

Args::Time Args::Clock::GetElapsedTime()
{
	return Time(std::chrono::system_clock::now() - start);
}

Args::Time Args::Clock::End()
{
	return Time(std::chrono::system_clock::now() - start);
}

Args::Time Args::Clock::Restart()
{
	auto startTime = std::chrono::system_clock::now();
	Time time = Time(startTime - start);
	start = startTime;
	return time;
}
