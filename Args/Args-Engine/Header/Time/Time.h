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

		double Minutes() { return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60>>>(time).count(); }
		double Seconds() { return std::chrono::duration_cast<std::chrono::duration<double>>(time).count(); }
		double Milliseconds() { return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time).count(); }
	};
}