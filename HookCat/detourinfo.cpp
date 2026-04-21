#include "detourinfo.h"

DetourInfo::DetourInfo()
{
	this->success = false;
	this->sourceAddr = 0;
	this->detourAddr = 0;
	this->returnAddr = 0;
}

DetourInfo::DetourInfo(uintptr_t sourceAddr, uintptr_t detourAddr)
{
	this->success = false;
	this->sourceAddr = sourceAddr;
	this->detourAddr = detourAddr;
	this->returnAddr = 0;
}

DetourInfo::operator int() const
{
	return success;
}
