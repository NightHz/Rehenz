#pragma once
#include <string>

namespace Rehenz
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef long long llong;
	typedef unsigned long long ullong;
#ifdef UNICODE
	typedef std::wstring String;
#else
	typedef std::string String;
#endif // UNICODE
}