#include "hookcat.h"

#include <Windows.h>
#include <Psapi.h>
#include "hextool.h"
#include "MinHook.h"
#include "mods.h"
#include "patcher.h"

namespace HookCat
{
	void HookCatMain::Initialize()
	{
		Kitten.Log.Info("Initializing...");

		if (MH_Initialize() != MH_OK)
		{
			Kitten.Log.Error("MinHook failed to initialize.");
			return;
		}

		if (mods.size() == 0)
			AddMods();

		Kitten.Log.Info("Starting HookCat...");

		Kitten.Log.Info("Loading mods...");
		for (auto& mod : mods)
			mod->OnLoad();

		Kitten.Log.Info("Loading mods complete.");
		Kitten.Log.Info("Initialization complete.");
	}

	void HookCatMain::Uninitialize()
	{
		Kitten.Log.Info("Uninitializing...");

		Kitten.Log.Info("Unloading mods...");
		for (auto& mod : mods)
			mod->OnUnload();

		if (MH_Uninitialize() != MH_OK)
		{
			Kitten.Log.Error("MinHook failed to uninitialize.");
		}

		Kitten.Log.Info("Unloading mods complete.");
		Kitten.Log.Info("Uninitialization complete.");
	}

	void HookCatMain::AddMod(std::shared_ptr<Mod> mod)
	{
		mods.emplace_back(mod);
	}

	std::optional<uintptr_t> HookCatMain::Find(const char* moduleName, const char* hexPattern)
	{
		auto pattern = HexTool::GetBytes(hexPattern);
		auto mask = HexTool::GetMask(hexPattern);

		pattern.push_back(0);
		mask.push_back(0);

		return Patcher::FindFirst(moduleName, pattern.data(), mask.data());
	}

	std::unique_ptr<std::vector<uintptr_t>> HookCatMain::FindAll(const char* moduleName, const char* hexPattern)
	{
		auto pattern = HexTool::GetBytes(hexPattern);
		auto mask = HexTool::GetMask(hexPattern);

		pattern.push_back(0);
		mask.push_back(0);

		return Patcher::FindAll(moduleName, pattern.data(), mask.data());
	}

	void HookCatMain::Patch(const char* moduleName, uintptr_t address, const char* hexWrite)
	{
		auto writeBytes = HexTool::GetBytes(hexWrite);
		auto mask = HexTool::GetMask(hexWrite);
		auto size = writeBytes.size();

		for (size_t i = 0; i < writeBytes.size(); ++i)
		{
			if (mask[i] == '?')
				writeBytes[i] = *(char*)(address + i);
		}

		Patcher::WriteToMemory(address, writeBytes.data(), size);
	}

	bool HookCatMain::Patch(const char* moduleName, const char* hexPattern, const char* hexWrite)
	{
		auto address = this->Find(moduleName, hexPattern);
		if (!address)
			return false;

		this->Patch(moduleName, address.value(), hexWrite);
		return true;
	}

	HookInfo HookCatMain::Hook(const char* moduleName, const char* funcName, void* detourFunc)
	{
		auto result = HookInfo(moduleName, funcName, detourFunc);

		auto moduleAddress = GetModuleHandle(moduleName);
		if (!moduleAddress)
		{
			return result;
		}

		auto funcAddress = GetProcAddress(moduleAddress, funcName);
		if (!funcAddress)
		{
			return result;
		}

		if (MH_CreateHook(funcAddress, detourFunc, &result.originalFunc) != MH_OK)
		{
			return result;
		}

		if (MH_EnableHook(funcAddress) != MH_OK)
		{
			return result;
		}

		result.success = true;

		return result;
	}

	bool HookCatMain::Unhook(HookInfo hookInfo)
	{
		return (MH_DisableHook(hookInfo.detourFunc) != MH_OK);
	}

	DetourInfo HookCatMain::Detour(uintptr_t sourceAddr, uintptr_t detourAddr, int overwriteCount)
	{
		auto result = DetourInfo(sourceAddr, detourAddr);

		auto addJumpAddr = sourceAddr;
		auto addJumpRet = sourceAddr + overwriteCount;

		std::vector<char> patchData(overwriteCount, '\x90');
		patchData[0] = '\xE9';
		*(int*)(&patchData[1]) = (int)detourAddr - (addJumpAddr + 5);

		Patcher::WriteToMemory(addJumpAddr, patchData.data(), patchData.size());

		result.success = true;
		result.returnAddr = addJumpRet;

		return result;
	}
}

HookCat::HookCatMain Kitten;
