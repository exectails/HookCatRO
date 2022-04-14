#include "hookinfo.h"

HookInfo::HookInfo()
{
	this->success = false;
	this->moduleName = nullptr;
	this->funcName = nullptr;
	this->detourFunc = nullptr;
}

HookInfo::HookInfo(const char* moduleName, const char* funcName, void* detourFunc)
{
	this->success = false;
	this->moduleName = moduleName;
	this->funcName = funcName;
	this->detourFunc = detourFunc;
}

HookInfo::operator int() const
{
	return success;
}
