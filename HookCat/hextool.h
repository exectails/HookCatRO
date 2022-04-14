#pragma once

#include <string>
#include <vector>
#include <Windows.h>

// Provides functions to convert from and to hex strings.
namespace HexTool
{
	// Converts the given string of hex characters to a list of bytes.
	// The function supports whitespaces, but all hex characters must
	// come in pairs.
	// Example:
	//     GetBytes("01 02  03 04 0506"); // { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }
	std::vector<char> GetBytes(const char* hexString);

	// Returns a mask pattern for the given string of hex characters,
	// where valid bytes are marked as "x", while placeholder bytes,
	// specified as "??", are marked as "?".
	// The function supports whitespaces, but all hex characters and
	// placeholders must come in pairs.
	// Example:
	//     GetMask("01 02  ?? ?? 0506"); // { 'x', 'x', '?', '?', 'x', 'x' }
	std::vector<char> GetMask(const char* hexString);

	// Converts value into a string of hex characters.
	std::string GetString(byte value);

	// Converts value into a string of hex characters.
	std::string GetString(int value);

	// Converts value into a string of hex characters.
	std::string GetString(float value);

	// Converts value into a string of hex characters.
	std::string GetString(const char* value, size_t size);
}
