#include <stdexcept>
#include "hextool.h"

// Provides functions to convert from and to hex strings.
namespace HexTool
{
	static const char characters[] = "0123456789ABCDEF";

	std::string byte2hex(byte input)
	{
		std::string result;

		result += characters[input >> 4];
		result += characters[input & 0x0F];

		return result;
	}

	byte hexchar2byte(char input)
	{
		if (input >= '0' && input <= '9')
			return input - '0';
		if (input >= 'A' && input <= 'F')
			return input - 'A' + 10;
		if (input >= 'a' && input <= 'f')
			return input - 'a' + 10;

		throw std::invalid_argument("Invalid input string");
	}

	std::vector<char> GetBytes(const char* hexString)
	{
		std::vector<char> bytes;

		auto src = hexString;
		while (*src && src[1])
		{
			if (*src == ' ')
			{
				src++;
				continue;
			}

			if (*src == '?' && src[1] == '?')
				bytes.push_back(0);
			else
				bytes.push_back((hexchar2byte(*src) << 4) | hexchar2byte(src[1]));

			src += 2;
		}

		return bytes;
	}

	std::vector<char> GetMask(const char* hexString)
	{
		std::vector<char> characters;

		auto src = hexString;
		while (*src && src[1])
		{
			if (*src == ' ')
			{
				src++;
				continue;
			}

			if (*src == '?' && src[1] == '?')
				characters.push_back('?');
			else
				characters.push_back('x');

			src += 2;
		}

		return characters;
	}

	std::string GetString(byte value)
	{
		auto ptr = reinterpret_cast<const char*>(&value);

		std::string result;

		result += byte2hex(ptr[0]);

		return result;
	}

	std::string GetString(int value)
	{
		auto ptr = reinterpret_cast<const char*>(&value);

		// Yes, this is inefficient, I'll change it some time when I feel
		// like bothering with it.

		std::string result;

		result += byte2hex(ptr[0]);
		result += byte2hex(ptr[1]);
		result += byte2hex(ptr[2]);
		result += byte2hex(ptr[3]);

		return result;
	}

	std::string GetString(float value)
	{
		auto ptr = reinterpret_cast<const char*>(&value);

		// Yes, this is inefficient, I'll change it some time when I feel
		// like bothering with it.

		std::string result;

		result += byte2hex(ptr[0]);
		result += byte2hex(ptr[1]);
		result += byte2hex(ptr[2]);
		result += byte2hex(ptr[3]);

		return result;
	}

	std::string GetString(const char* value, size_t size)
	{
		std::string result;

		for (int i = 0; i < (int)size; ++i)
		{
			result += byte2hex(value[i]);
		}

		return result;
	}
}
