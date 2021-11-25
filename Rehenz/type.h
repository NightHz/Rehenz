#pragma once
#include <string>

namespace Rehenz
{
	typedef unsigned int uint;
#ifdef UNICODE
	typedef std::wstring String;
#else
	typedef std::string String;
#endif // UNICODE
}