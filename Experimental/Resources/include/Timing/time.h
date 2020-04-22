#pragma once
#include <chrono>

namespace Args
{
	template<typename time_type>
	class time
	{
	private:
		std::chrono::duration<time_type> duration;

	public:
		time(std::chrono::duration<time_type> duration) : duration(duration) {}

		template<typename T>
		T minutes() { return std::chrono::duration_cast<std::chrono::duration<T, std::ratio<60>>>(duration).count(); }
		template<typename T>
		T seconds() { return std::chrono::duration_cast<std::chrono::duration<T>>(duration).count(); }
		template<typename T>
		T milliseconds() { return std::chrono::duration_cast<std::chrono::duration<T, std::milli>>(duration).count(); }

		float minutes() { return std::chrono::duration_cast<std::chrono::duration<time_type, std::ratio<60>>>(duration).count(); }
		float seconds() { return std::chrono::duration_cast<std::chrono::duration<time_type>>(duration).count(); }
		float milliseconds() { return std::chrono::duration_cast<std::chrono::duration<time_type, std::milli>>(duration).count(); }
	};
}