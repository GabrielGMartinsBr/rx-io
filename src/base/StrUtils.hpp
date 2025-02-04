#pragma once

#include <string>

typedef std::string Str;
typedef const char* CStr;

namespace StrUtils {

constexpr unsigned int hash(const char* str, int h = 0)
{
  return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

}  // namespace StrUtils
