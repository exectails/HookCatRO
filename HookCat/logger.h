#pragma once

#include <fstream>

// Provides functions to log messages to a file.
class Logger
{
	std::ofstream logFile;

	// Writes message to log file, prefixed with the current time and the tag.
	void writeln(const char* tag, const char* format, va_list args);

	// Writes message to log file, prefixed with the current time and the tag.
	void writeln(const wchar_t* tag, const wchar_t* format, va_list args);

public:
	// Creates new instance that writes to the given file.
	// Example:
	//     auto log = Logger("./dinput8.ini");
	Logger(const char* fileName);

	// Cleans up acquired resources.
	~Logger();

	// Logs an info messsage using printf formatting.
	// Example
	//     Info("Foobar: %d", intVal);
	void Info(const char* format, ...);

	// Logs a warning message using printf formatting.
	// Example
	//     Warning("Foobar: %d", intVal);
	void Warning(const char* format, ...);

	// Logs an error message using printf formatting.
	// Example
	//     Error("Foobar: %d", intVal);
	void Error(const char* format, ...);

	// Logs a debug message using printf formatting.
	// Example
	//     Debug("Foobar: %d", intVal);
	void Debug(const char* format, ...);

	// Logs a debug message using printf formatting.
	// Example
	//     Debug("Foobar: %d", args);
	void Debug(const wchar_t* format, va_list args);
};
