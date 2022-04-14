#include <optional>
#include <memory>
#include <Windows.h>
#include <Psapi.h>
#include "hextool.h"

namespace Patcher
{
	std::optional<size_t> getModuleSize(HMODULE module)
	{
		if (!module)
			return {};

		MODULEINFO modinfo;
		GetModuleInformation(GetCurrentProcess(), module, &modinfo, sizeof(MODULEINFO));

		return modinfo.SizeOfImage;
	}

	std::optional<size_t> getModuleSize(const char* moduleName)
	{
		auto module = GetModuleHandle(moduleName);
		return getModuleSize(module);
	}

	std::optional<uintptr_t> FindFirst(const char* moduleName, const char* pattern, const char* mask)
	{
		auto module = GetModuleHandle(moduleName);
		auto base = (uintptr_t)module;

		auto size = getModuleSize(module);
		if (!size)
			return NULL;

		auto patternLength = (DWORD)strlen(mask);

		for (DWORD i = 0; i < size.value_or(0) - patternLength; i++)
		{
			auto found = true;

			for (DWORD j = 0; j < patternLength; j++)
			{
				found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
			}

			if (found)
			{
				return base + i;
			}
		}

		return {};
	}

	std::optional<uintptr_t> FindFirst(const char* moduleName, const char* hexPattern)
	{
		auto pattern = HexTool::GetBytes(hexPattern);
		auto mask = HexTool::GetMask(hexPattern);

		pattern.push_back(0);
		mask.push_back(0);

		return FindFirst(moduleName, pattern.data(), mask.data());
	}

	std::unique_ptr<std::vector<uintptr_t>> FindAll(const char* moduleName, const char* pattern, const char* mask)
	{
		auto result = std::make_unique<std::vector<uintptr_t>>();

		auto module = GetModuleHandle(moduleName);
		auto base = (uintptr_t)module;

		auto size = getModuleSize(module);
		if (!size)
			return result;

		auto patternLength = (DWORD)strlen(mask);

		for (DWORD i = 0; i < size.value_or(0) - patternLength; i++)
		{
			auto found = true;

			for (DWORD j = 0; j < patternLength; j++)
			{
				found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
			}

			if (found)
			{
				//return base + i;
				result->push_back(base + i);
			}
		}

		return result;
	}

	std::unique_ptr<std::vector<uintptr_t>> FindAll(const char* moduleName, const char* hexPattern)
	{
		auto pattern = HexTool::GetBytes(hexPattern);
		auto mask = HexTool::GetMask(hexPattern);

		pattern.push_back(0);
		mask.push_back(0);

		return FindAll(moduleName, pattern.data(), mask.data());
	}

	void WriteToMemory(uintptr_t addr, const char* buffer, size_t size)
	{
		unsigned long oldProtect;
		VirtualProtect((LPVOID)(addr), size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy((LPVOID)addr, buffer, size);
		VirtualProtect((LPVOID)(addr), size, oldProtect, NULL);
	}

	void Patch(uintptr_t address, const char* hexWrite)
	{
		auto writeBytes = HexTool::GetBytes(hexWrite);
		auto size = writeBytes.size();

		WriteToMemory(address, writeBytes.data(), size);
	}

	bool Patch(const char* moduleName, const char* hexPattern, const char* hexWrite)
	{
		auto address = FindFirst(moduleName, hexPattern);
		if (!address)
			return false;

		Patch(address.value(), hexWrite);
		return true;
	}
}
