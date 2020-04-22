#pragma once
#include <iostream>
#include <Types/primitives.h>

namespace Args
{
	class Console
	{
	public:
		template<typename... Arguments>
		static inline void log(const char* message, Arguments... arguments)
		{
			std::printf(message, arguments...);
		}

		template<typename... Arguments>
		static inline void log(string message, Arguments... arguments)
		{
			std::printf(message.c_str(), arguments...);
		}

		template<typename some_type>
		static inline void log(some_type message)
		{
			std::cout << message;
		}

		template<typename... Arguments>
		static inline void logln(const char* message, Arguments... arguments)
		{
			std::printf(message, arguments...);
			std::cout << std::endl;
		}

		template<typename... Arguments>
		static inline void logln(string message, Arguments... arguments)
		{
			std::printf(message, arguments...);
			std::cout << std::endl;
		}

		template<typename some_type>
		static inline void logln(some_type message)
		{
			std::cout << message << std::endl;
		}
	};
}