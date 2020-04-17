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

		template<typename T>
		T Minutes() { return std::chrono::duration_cast<std::chrono::duration<T, std::ratio<60>>>(time).count(); }
		template<typename T>
		T Seconds() { return std::chrono::duration_cast<std::chrono::duration<T>>(time).count(); }
		template<typename T>
		T Milliseconds() { return std::chrono::duration_cast<std::chrono::duration<T, std::milli>>(time).count(); }

		float Minutes() { return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<60>>>(time).count(); }
		float Seconds() { return std::chrono::duration_cast<std::chrono::duration<float>>(time).count(); }
		float Milliseconds() { return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(time).count(); }
	};
}