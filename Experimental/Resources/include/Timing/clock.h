#pragma once
#include <chrono>
#include <Timing/time.h>
#include <Types/time_types.h>

namespace Args
{


	template<typename time_type = time64>
	class clock
	{
	private:
		std::chrono::time_point<std::chrono::system_clock> start;
	public:
		void Start() {
			start = std::chrono::system_clock::now();
		}

		time<time_type> GetElapsedTime()
		{
			return time<time_type>(std::chrono::system_clock::now() - start);
		}

		time<time_type> End()
		{
			return time<time_type>(std::chrono::system_clock::now() - start);
		}

		time<time_type> Restart()
		{
			auto startTime = std::chrono::system_clock::now();
			time<time_type> time = time<time_type>(startTime - start);
			start = startTime;
			return time;
		}
	};
}
