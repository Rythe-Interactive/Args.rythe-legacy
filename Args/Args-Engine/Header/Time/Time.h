#pragma once
#include <chrono>

namespace Args
{
	class Time
	{
	private:
		std::chrono::duration<double> time;

	public:
		Time(std::chrono::duration<double> time) : time(time){}

		float Minutes() { return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<60>>>(time).count(); }
		float Seconds() { return std::chrono::duration_cast<std::chrono::duration<float>>(time).count(); }
		float Milliseconds() { return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(time).count(); }
	};
}