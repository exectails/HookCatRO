#include "logger.h"

#include <stdarg.h> 
#include <vector>
#include "date.h"

Logger::Logger(const char* fileName)
{
	logFile = std::ofstream(fileName, std::ios_base::out);
}

Logger::~Logger()
{
	if (logFile)
		logFile.close();
}

void Logger::writeln(const char* tag, const char* format, va_list args)
{
	auto size = vsnprintf(NULL, 0, format, args);
	std::vector<char> textBuffer(size + 1);
	std::vsnprintf(&textBuffer[0], textBuffer.size(), format, args);

	auto dateStr = date::format("%Y-%m-%d %H:%M:%S", std::chrono::system_clock::now());

	logFile << dateStr << " [" << tag << "] - " << textBuffer.data() << std::endl;
}

void Logger::writeln(const wchar_t* tag, const wchar_t* format, va_list args)
{
	auto size = vswprintf(NULL, 0, format, args);
	std::vector<wchar_t> textBuffer(size + 1);
	std::vswprintf(&textBuffer[0], textBuffer.size(), format, args);

	auto dateStr = date::format("%Y-%m-%d %H:%M:%S", std::chrono::system_clock::now());

	logFile << dateStr << " [" << tag << "] - " << textBuffer.data() << std::endl;
}

void Logger::Info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	writeln("nfo", format, args);
	va_end(args);
}

void Logger::Warning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	writeln("wrn", format, args);
	va_end(args);
}

void Logger::Error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	writeln("err", format, args);
	va_end(args);
}

void Logger::Debug(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	writeln("dbg", format, args);
	va_end(args);
}

void Logger::Debug(const wchar_t* format, va_list args)
{
	writeln(L"dbg", format, args);
}
