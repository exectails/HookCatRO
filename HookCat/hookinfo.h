#pragma once

class HookInfo
{
public:
	bool success;

	const char* moduleName;
	const char* funcName;

	void* originalFunc;
	void* detourFunc;

	HookInfo();
	HookInfo(const char* moduleName, const char* funcName, void* detourFunc);
	operator int() const;
};
