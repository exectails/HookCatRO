#include "util.h"

bool IsEmptyOrWhitespace(const std::string& str)
{
	auto it = str.begin();
	do
	{
		if (it == str.end())
			return true;
	} while (std::isspace(*(it++)));

	return false;
}
