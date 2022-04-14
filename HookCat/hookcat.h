#pragma once

#include <optional>
#include <vector>
#include "hookinfo.h"
#include "ini.h"
#include "logger.h"
#include "mod.h"

namespace HookCat
{
	// Main class holding common hooking, patching, and utility methods.
	class HookCatMain
	{
		std::vector<std::shared_ptr<Mod>> mods;

	public:
		// Reference to application's configuration.
		Ini Conf = Ini("./dinput8.ini");

		// Reference to logger that writes to the central application log file.
		Logger Log = Logger("./dinput8.log");

		// Initializes HookCat, loading all mods.
		void Initialize();

		// Uninitializes HookCat, unloading all mods.
		void Uninitialize();

		// Adds mod to list of mods to load and unload.
		// Example:
		//     AddMod(std::make_shared<MyMod>());
		void AddMod(std::shared_ptr<Mod> mod);

		// Searches for the pattern in the module and returns its address
		// if it was found.
		// Example:
		//     auto address = Find("Foo.dll", "3BF1 72 10");
		//     if (address)
		//         Patch("Foo.dll", address.value(), "3BF1 EB 10");
		std::optional<uintptr_t> Find(const char* moduleName, const char* hexPattern);

		// Searches for the pattern in the module and returns its address
		// if it was found.
		// Example:
		//     auto addresses = Find("Foo.dll", "3BF1 72 10");
		//     if (addresses->size != 0)
		//         Patch("Foo.dll", address.value(), "3BF1 EB 10");
		std::unique_ptr<std::vector<uintptr_t>> FindAll(const char* moduleName, const char* hexPattern);

		// Writes the bytes in the hex string to the first address found
		// for the pattern.
		// Example:
		//     Patch("Foo.dll", "3BF1 72 10", "3BF1 EB 10");
		bool Patch(const char* moduleName, const char* hexPattern, const char* hexWrite);

		// Writes the bytes in the hex string to the address.
		// Example:
		//     auto address = Find("Foo.dll", "3BF1 72 10");
		//     if (address)
		//         Patch("Foo.dll", address.value(), "3BF1 EB 10");
		void Patch(const char* moduleName, uintptr_t address, const char* hexWrite);

		// Creates and returns a hook for the function in the module.
		// Example:
		//     HookInfo hook;
		//     
		//     void __fastcall foobar(void* self, float a1, float a2)
		//     {
		//         ((decltype(foobar)*)hook.originalFunc)(self, a1, a2);
		//     }
		//     
		//     hook = Hook("Foo.dll", "?Bar@Foo@@UAEXMM@Z", &foobar);
		//     if (!hook)
		//         log.error("Hook failed");
		HookInfo Hook(const char* moduleName, const char* funcName, void* detourFunc);

		// Disables the given hook.
		bool Unhook(HookInfo hookInfo);
	};
}

// Global instance of HookCatMain for access to all important functions
// and properties.
extern HookCat::HookCatMain Kitten;
