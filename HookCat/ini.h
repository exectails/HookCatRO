#pragma once

#include <string>

// Provides functions to read options from an ini file.
class Ini
{
	const char* fileName;

public:
	// Creates new instance that reads from the given file.
	// Example:
	//    auto ini = Ini("./dinput8.ini");
	Ini(const char* fileName);

	// Returns an option's value. Returns an empty string if the
	// option doesn't exist.
	std::string GetString(const char* category, const char* name);

	// Returns an option's value. Returns an empty string if the
	// option doesn't exist.
	std::string GetString(const char* category, const char* namey, const char* defaultValue);

	// Returns an option's value, converted to a boolean. Returns default
	// value if the option was empty or didn't exist.
	bool GetBool(const char* category, const char* name, bool defaultValue = false);

	// Returns an option's value, converted to an int. Returns default
	// value if the option was empty or didn't exist.
	int GetInt(const char* category, const char* name, int defaultValue = 0);

	// Returns an option's value, converted to a float. Returns default
	// value if the option was empty or didn't exist.
	float GetFloat(const char* category, const char* name, float defaultValue = 0);
};
