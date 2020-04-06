#pragma once
#include <stdio.h>
#include <time.h>
#include "Defaults.h"
#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include <Windows.h>

//#define LOG_RELEASE

#ifdef _DEBUG
#define LOG_CONSOLE
#else
#ifdef LOG_RELEASE
#define LOG_CONSOLE
#endif // LOG_RELEASE
#endif

#ifdef LOG_CONSOLE
namespace Args
{
	static HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	static HANDLE hConsoleErr = GetStdHandle(STD_ERROR_HANDLE);

	static int defTextCol = DEBUG_WHITE;
	static const int defErrCol = DEBUG_RED;
	static const int defWarnCol = DEBUG_YELLOW;
	static const int defSuccessCol = DEBUG_GREEN;
	static int textCol = defTextCol;
	static int errCol = defErrCol;
	static int warnCol = defWarnCol;
	static int successCol = defSuccessCol;
}

#define SET_DEBUG_COLOR(consoleHandle, printMode, textColor)			\
	switch (printMode)													\
	{																	\
		case DEFAULT:													\
			SetConsoleTextAttribute(consoleHandle, textColor);			\
			Args::defTextCol = textColor;								\
			break;														\
		case TEMP:														\
			SetConsoleTextAttribute(consoleHandle, textColor);			\
			Args::textCol = textColor;									\
			break;														\
		case SUCCESS:													\
			Args::successCol = textColor;								\
			break;														\
		case ERROR:														\
			SetConsoleTextAttribute(consoleHandle, textColor);			\
			Args::errCol = textColor;									\
			break;														\
		case WARNING:													\
			Args::warnCol = textColor;									\
			break;														\
	}																	\

#define RESET_DEBUG_COLOR(printMode)									\
	switch (printMode)													\
	{																	\
		case DEFAULT:													\
			SET_DEBUG_COLOR(hConsoleOut, printMode, Args::defTextCol)	\
			break;														\
		case TEMP:														\
			SET_DEBUG_COLOR(hConsoleOut, printMode, Args::defTextCol)	\
			break;														\
		case SUCCESS:													\
			SET_DEBUG_COLOR(hConsoleOut, printMode, Args::defSuccessCol)\
			break;														\
		case ERROR:														\
			SET_DEBUG_COLOR(hConsoleErr, printMode, Args::defErrCol)	\
			break;														\
		case WARNING:													\
			SET_DEBUG_COLOR(hConsoleOut, printMode, Args::defWarnCol)	\
			break;														\
	}																	\

#define CREATE_MESSAGE(data, debugMessage)																														\
	std::string(std::string("[") + data.file + std::string("]\tline ") + data.line + std::string(": ") + std::string(debugMessage) + std::string("\n")).c_str()	\

#define PRINT_MESSAGE(data, debugMessage, tempTextCol, inserts)				\
{																			\
	SET_DEBUG_COLOR(Args::hConsoleOut, TEMP, tempTextCol)					\
	std::fprintf(stdout, CREATE_MESSAGE(data, debugMessage), inserts...);	\
	RESET_DEBUG_COLOR(TEMP)													\
}

#define PRINT_SUCCESS(data, debugMessage, inserts)							\
	std::fprintf(stdout, "\n");												\
	PRINT_MESSAGE(data, debugMessage, Args::successCol, inserts);			\
	std::fprintf(stdout, "\n");												\

#define PRINT_ERR(data, errorMessage, inserts)								\
{																			\
	SetConsoleTextAttribute(hConsoleErr, Args::errCol);						\
	std::fprintf(stderr, "\n");												\
	std::fprintf(stderr, CREATE_MESSAGE(data, errorMessage), inserts...);	\
	std::fprintf(stderr, "\n");												\
}

#define PRINT_WARN(data, warningMessage, inserts)						\
	std::fprintf(stdout, "\n");											\
	PRINT_MESSAGE(data, warningMessage, Args::warnCol, inserts);		\
	std::fprintf(stdout, "\n");											\

#define PRINT(printMode, data, message, inserts)						\
	switch (printMode)													\
	{																	\
		case SUCCESS:													\
			PRINT_SUCCESS(data, message, inserts)						\
			break;														\
		case ERROR:														\
			PRINT_ERR(data, message, inserts)							\
			break;														\
		case WARNING:													\
			PRINT_WARN(data, message, inserts)							\
			break;														\
		case DEFAULT:													\
			PRINT_MESSAGE(data, message, Args::defTextCol, inserts)		\
			break;														\
		default:														\
			PRINT_MESSAGE(data, message, printMode, inserts)			\
	}																	\

#else

#define SET_DEBUG_COLOR(consoleHandle, printMode, textColor) {}
#define RESET_DEBUG_COLOR(printMode) {}

#define CREATE_MESSAGE(data, debugMessage)																														\
	std::string(std::string("[") + data.file + std::string("]\tline ") + data.line + std::string(": ") + std::string(debugMessage) + std::string("\n")).c_str()	\

#define PRINT_MESSAGE(data, debugMessage, tempTextCol, inserts) {}
#define PRINT_SUCCESS(data, debugMessage, inserts) {}
#define PRINT_ERR(data, errorMessage, inserts) {}
#define PRINT_WARN(data, warningMessage, inserts) {}
#define PRINT(printMode, data, message, inserts) {}
#endif

#define DebugInfo Args::Debug::DebugData(__FILE__, __LINE__)


namespace Args
{
	class Debug
	{
	private:
		static std::string startDate;
		static FILE* outFile;
	public:
		struct DebugData
		{
		public:
			std::string file;
			std::string line;

			DebugData(const char* fileName, int lineNumber) : file(fileName), line(std::to_string(lineNumber))
			{
				file = file.substr(file.find_last_of("\\") + 1);
			}
		};

		static std::string GetDateTime()
		{
			time_t rawtime;
			struct tm timeinfo;
			char buffer[80];

			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);

			strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M", &timeinfo);

			return std::string(buffer);
		}

		inline static void CloseOutputFile()
		{
			if (!outFile)
				return;

			fclose(outFile);
			outFile = nullptr;
		}

		inline static void OpenOutputFile()
		{
			fopen_s(&outFile, (std::string("../Logs/output ") + startDate + std::string(".log")).c_str(), "w");
		}

		inline static void SetColor(int type, int color)
		{
			if (type == ERROR)
				SET_DEBUG_COLOR(hConsoleErr, type, color)
			else
				SET_DEBUG_COLOR(hConsoleOut, type, color)
		}

		inline static void SetColor(int type, bool r, bool g, bool b, bool intensify)
		{
			int color = (r ? 0x0001 : 0x0) | (g ? 0x0010 : 0x0) | (b ? 0x0100 : 0x0) | (intensify ? 0x1000 : 0x0);

			if (type == ERROR)
				SET_DEBUG_COLOR(hConsoleErr, type, color)
			else
				SET_DEBUG_COLOR(hConsoleOut, type, color)
		}

		inline static void ResetColor(int type)
		{
			RESET_DEBUG_COLOR(type)
		}

		template<typename... InsertTypes>
		inline static void Log(int color, DebugData data, const char* message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, CREATE_MESSAGE(data, message), inserts...);

			PRINT_MESSAGE(data, message, color, inserts)
		}

		template<typename... InsertTypes>
		inline static void Log(int color, DebugData data, std::string message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, CREATE_MESSAGE(data, message.c_str()), inserts...);

			PRINT_MESSAGE(data, message.c_str(), color, inserts)
		}

		template<typename... InsertTypes>
		inline static void Log(DebugData data, const char* message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, CREATE_MESSAGE(data, message), inserts...);

			PRINT(DEFAULT, data, message, inserts)
		}

		template<typename... InsertTypes>
		inline static void Log(DebugData data, std::string message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, CREATE_MESSAGE(data, message.c_str()), inserts...);

			PRINT(DEFAULT, data, message.c_str(), inserts)
		}

		template<typename... InsertTypes>
		inline static void Success(DebugData data, const char* message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, "\n");
			std::fprintf(outFile, CREATE_MESSAGE(data, message), inserts...);
			std::fprintf(outFile, "\n");

			PRINT(SUCCESS, data, message, inserts)
		}

		template<typename... InsertTypes>
		inline static void Success(DebugData data, std::string message, InsertTypes... inserts)
		{
			Debug::Success(data, message.c_str(), inserts...);
		}

		template<typename... InsertTypes>
		inline static void Error(DebugData data, const char* message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, "\n<[ERROR]> ");
			std::fprintf(outFile, CREATE_MESSAGE(data, message), inserts...);
			std::fprintf(outFile, "\n");

			PRINT(ERROR, data, message, inserts)

			char log[100];

			std::snprintf(log, 100, CREATE_MESSAGE(data, message), inserts...);

#ifdef _DEBUG
			throw std::logic_error(std::string(log));
#endif
		}

		template<typename... InsertTypes>
		inline static void Error(DebugData data, std::string message, InsertTypes... inserts)
		{
			Debug::Error(data, message.c_str(), inserts...);
		}

		template<typename... InsertTypes>
		inline static void Warning(DebugData data, const char* message, InsertTypes... inserts)
		{
			if (!outFile)
				OpenOutputFile();

			std::fprintf(outFile, "\n<[WARNING]> ");
			std::fprintf(outFile, CREATE_MESSAGE(data, message), inserts...);
			std::fprintf(outFile, "\n");

			PRINT(WARNING, data, message, inserts)
		}

		template<typename... InsertTypes>
		inline static void Warning(DebugData data, std::string message, InsertTypes... inserts)
		{
			Debug::Warning(data, message.c_str(), inserts...);
		}
	};
}