#pragma once

#include <optional>

namespace Patcher
{
	// Finds the first address at which the given pattern was found in the
	// module.
	// Example:
	//      // Find instruction regardless of numeric value
	//     auto address = FindFirst("Foo.dll", "C7 44 24 08 00 00 C8 42", "xxxx????");
	//     if (address)
	//         // found!
	std::optional<uintptr_t> FindFirst(const char* moduleName, const char* pattern, const char* mask);

	// Finds the first address at which the given pattern was found in the
	// module. Unlike the alternative FindFirst it takes a hex string that
	// combines pattern and mask.
	// Example:
	//      // Find instruction regardless of numeric value
	//     auto address = Find("Foo.dll", "C7 44 24 08 ?? ?? ?? ??");
	//     if (address)
	//         // found!
	std::optional<uintptr_t> FindFirst(const char* moduleName, const char* hexPattern);

	// Finds the first address at which the given pattern was found in the
	// module.
	// Example:
	//      // Find instruction regardless of numeric value
	//     auto address = FindFirst("Foo.dll", "C7 44 24 08 00 00 C8 42", "xxxx????");
	//     if (address)
	//         // found!
	std::unique_ptr<std::vector<uintptr_t>> FindAll(const char* moduleName, const char* pattern, const char* mask);

	// Finds the first address at which the given pattern was found in the
	// module. Unlike the alternative FindFirst it takes a hex string that
	// combines pattern and mask.
	// Example:
	//      // Find instruction regardless of numeric value
	//     auto address = Find("Foo.dll", "C7 44 24 08 ?? ?? ?? ??");
	//     if (address)
	//         // found!
	std::unique_ptr<std::vector<uintptr_t>> FindAll(const char* moduleName, const char* hexPattern);

	// Writes buffer to address.
	// Example:
	//     auto address = FindFirst("Foo.dll", "C7 44 24 08 00 00 C8 42", "xxxx????");
	//     if (address)
	//         WriteToMemory(address, "\xC7\x44\x24\x08\x00\x00\xFF\xFF", 8);
	void WriteToMemory(uintptr_t addr, const char* buffer, size_t size);

	// Patches the first occurence of the pattern in the module, writing the
	// bytes in the hex string to the found address.
	// Example:
	//     if (!Patch("Foo.dll", "C7 44 24 08 ?? ?? ?? ??", "C7 44 24 08 00 00 FF FF"))
	//         // patch failed
	bool Patch(const char* moduleName, const char* hexPattern, const char* hexWrite);

	// Writes the in hex string to the given address.
	// Example:
	//      // Find instruction regardless of numeric value
	//     auto address = Find("Foo.dll", "C7 44 24 08 ?? ?? ?? ??");
	//     if (address)
	//         Patch(address, "C7 44 24 08 00 00 FF FF");
	void Patch(uintptr_t address, const char* hexWrite);
}
