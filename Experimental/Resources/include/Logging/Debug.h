#pragma once
#include <Logging/Console.h>
#include <stdexcept>

namespace Args
{
	class Debug
	{
	public:
		static void assert(bool condition)
		{
			if (!condition)
				throw std::logic_error("");
		}
		static void assert(bool condition, const char* message)
		{
			if (!condition)
			{
				Console::log(message);
				throw std::logic_error(message);
			}
		}
		static void assert(bool condition, string message)
		{
			if (!condition)
			{
				Console::log(message);
				throw std::logic_error(message);
			}
		}
		template<typename... Arguments>
		static void assert(bool condition, const char* message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments);
				throw std::logic_error("");
			}
		}
		template<typename... Arguments>
		static void assert(bool condition, string message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments);
				throw std::logic_error("");
			}
		}
	};
}