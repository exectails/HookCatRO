#pragma once

#include <cctype>
#include <string>

// Returns true if the string is either empty or contains only
// whitespace characters.
// Example:
//     IsWhitespace(""); // true
//     IsWhitespace(" "); // true
//     IsWhitespace("\t"); // true
//     IsWhitespace("a"); // false
//     IsWhitespace(" a "); // false
bool IsEmptyOrWhitespace(const std::string& str);
