#include <algorithm>
#include <Windows.h>
#include "ini.h"

const int MAX_LINE_LENGTH = 1024;

Ini::Ini(const char* fileName)
{
	this->fileName = fileName;
}

std::string Ini::GetString(const char* category, const char* name)
{
	char out[MAX_LINE_LENGTH];
	GetPrivateProfileString(category, name, "", out, sizeof(out), fileName);

	return std::string(out);
}

std::string Ini::GetString(const char* category, const char* name, const char* defaultValue)
{
	auto str = GetString(category, name);
	if (str == "")
		return defaultValue;

	return str;
}

bool Ini::GetBool(const char* category, const char* name, bool defaultValue)
{
	auto str = GetString(category, name);
	if (str == "")
		return defaultValue;

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str == "true" || str == "yes";
}

int Ini::GetInt(const char* category, const char* name, int defaultValue)
{
	auto str = GetString(category, name);
	if (str == "")
		return defaultValue;

	return atoi(str.c_str());
}

float Ini::GetFloat(const char* category, const char* name, float defaultValue)
{
	auto str = GetString(category, name);
	if (str == "")
		return defaultValue;

	return (float)atof(str.c_str());
}
