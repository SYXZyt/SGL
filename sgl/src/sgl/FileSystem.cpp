#include "FileSystem.h"
#include <filesystem>
#include <sgl/Error.h>
#include <fstream>

bool sgl::FileSystem::ValidateFile(const String& path)
{
    if (!std::filesystem::exists(path))
    {
        std::string p = ConvertString<String, std::string>(path);

        SGL_REPORT_ERROR(FormatString(u8"File does not exist: {}", p.c_str()));
        return false;
    }

    return true;
}

sgl::String sgl::FileSystem::ReadFile(const String& path)
{
    if (!ValidateFile(path))
        return u8"";

    std::ifstream in(ConvertString<String, std::string>(path));

    if (!in)
        return u8"";

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return ConvertString<std::string, String>(content);
}

bool sgl::FileSystem::ReadFileToLines(const String& path, List<String>& outLines)
{
    if (!ValidateFile(path))
        return false;

    outLines.clear();

    String contents = ReadFile(path);

    std::stringstream ss(ConvertString<String, std::string>(contents));
    std::string line;

    while (std::getline(ss, line, '\n'))
        outLines.push_back(ConvertString<std::string, String>(line));

    return true;
}

bool sgl::FileSystem::ReadFileToBinary(const String& path, List<byte>& outBinary)
{
    if (!ValidateFile(path))
        return false;

    std::ifstream file(ConvertString<String, std::string>(path), std::ios::in | std::ios::binary);
    if (!file)
    {
        SGL_REPORT_ERROR(u8"Failed to open file: " + path);
        return false;
    }

    outBinary.clear();

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    outBinary.resize(size);

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(outBinary.data()), size);
    file.close();

    return true;
}