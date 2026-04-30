#pragma once
#include <sgl/sgl.h>
#include <sgl/String.h>
#include <sgl/Collections/List.h>

namespace sgl::FileSystem
{
    SGL_API extern bool ValidateFile(const String& path);
    SGL_API extern String ReadFile(const String& path);

    SGL_API extern bool ReadFileToLines(const String& path, List<String>& outLines);
    SGL_API extern bool ReadFileToBinary(const String& path, List<byte>& outBinary);
}