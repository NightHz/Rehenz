#pragma once
#include "type.h"

namespace Rehenz
{
    // require link ole32.lib

    // ouput format : high-(A,R,G,B)-low = 32bppBGRA
    std::unique_ptr<uint[]> LoadImageFile(const std::wstring& filename, uint& width, uint& height);

}