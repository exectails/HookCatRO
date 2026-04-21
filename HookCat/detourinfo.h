#pragma once

#include <Windows.h>

class DetourInfo
{
public:
	bool success;

	uintptr_t sourceAddr;
	uintptr_t detourAddr;
	uintptr_t returnAddr;

	DetourInfo();
	DetourInfo(uintptr_t sourceAddr, uintptr_t detourAddr);
	operator int() const;
};
