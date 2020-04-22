#pragma once
#include <Logging/Console.h>
#include <stdexcept>

namespace Args
{
	class Debug
	{
	public:
		static inline void soft_assert(bool condition, const char* message)
		{
			if (!condition)
			{
				Console::log(message);
			}
		}

		static inline void soft_assert(bool condition, string message)
		{
			if (!condition)
			{
				Console::log(message);
			}
		}

		template<typename... Arguments>
		static inline void soft_assert(bool condition, const char* message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments...);
			}
		}

		template<typename... Arguments>
		static inline void soft_assert(bool condition, string message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments...);
			}
		}

		static inline void assert(bool condition)
		{
			if (!condition)
				throw std::logic_error("");
		}

		static inline void assert(bool condition, const char* message)
		{
			if (!condition)
			{
				Console::log(message);
				throw std::logic_error(message);
			}
		}

		static inline void assert(bool condition, string message)
		{
			if (!condition)
			{
				Console::log(message);
				throw std::logic_error(message);
			}
		}

		template<typename... Arguments>
		static inline void assert(bool condition, const char* message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments...);
				throw std::logic_error("");
			}
		}

		template<typename... Arguments>
		static inline void assert(bool condition, string message, Arguments... arguments)
		{
			if (!condition)
			{
				Console::log(message, arguments...);
				throw std::logic_error("");
			}
		}
	};
}