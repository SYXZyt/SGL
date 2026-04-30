#include "String.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <cwchar>
#endif

#ifdef _WIN32
#define HRESULT_TO_STRING(hr) case hr: return #hr

const char* sgl::HResultToString(HRESULT hr)
{
	switch (hr)
	{
		HRESULT_TO_STRING(S_OK);
		HRESULT_TO_STRING(S_FALSE);
		HRESULT_TO_STRING(E_FAIL);
		HRESULT_TO_STRING(E_INVALIDARG);
		HRESULT_TO_STRING(E_OUTOFMEMORY);
		HRESULT_TO_STRING(E_NOTIMPL);
		HRESULT_TO_STRING(E_POINTER);
		HRESULT_TO_STRING(E_ACCESSDENIED);
		HRESULT_TO_STRING(E_HANDLE);
		HRESULT_TO_STRING(E_ABORT);

		HRESULT_TO_STRING(DXGI_ERROR_INVALID_CALL);
		HRESULT_TO_STRING(DXGI_ERROR_NOT_FOUND);
		HRESULT_TO_STRING(DXGI_ERROR_MORE_DATA);
		HRESULT_TO_STRING(DXGI_ERROR_UNSUPPORTED);
		HRESULT_TO_STRING(DXGI_ERROR_DEVICE_REMOVED);
		HRESULT_TO_STRING(DXGI_ERROR_DEVICE_HUNG);
		HRESULT_TO_STRING(DXGI_ERROR_DEVICE_RESET);
		HRESULT_TO_STRING(DXGI_ERROR_DRIVER_INTERNAL_ERROR);
		HRESULT_TO_STRING(DXGI_ERROR_FRAME_STATISTICS_DISJOINT);
		HRESULT_TO_STRING(DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE);
		HRESULT_TO_STRING(DXGI_ERROR_WAS_STILL_DRAWING);
		HRESULT_TO_STRING(DXGI_ERROR_NONEXCLUSIVE);
		HRESULT_TO_STRING(DXGI_ERROR_NOT_CURRENTLY_AVAILABLE);
		HRESULT_TO_STRING(DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED);
		HRESULT_TO_STRING(DXGI_ERROR_REMOTE_OUTOFMEMORY);

		HRESULT_TO_STRING(D3D11_ERROR_FILE_NOT_FOUND);
		HRESULT_TO_STRING(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);
		HRESULT_TO_STRING(D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS);
		HRESULT_TO_STRING(D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD);

		default:
			return "Unknown error code";
	}
}
#endif

std::string sgl::ToString(const std::u8string& str)
{
	if (str.empty())
		return "";

	return std::string(reinterpret_cast<const char*>(str.data()), str.size());
}

std::string sgl::ToString(const std::wstring& str)
{
    if (str.empty())
        return "";

#ifdef _WIN32

    int size = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);

    std::string result(size, 0);

    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size, nullptr, nullptr);

    return result;

#else

    std::mbstate_t state{};
    const wchar_t* src = str.data();

    size_t size = wcsrtombs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::string result(size, 0);

    state = std::mbstate_t{};
    src = str.data();

    wcsrtombs(result.data(), &src, size, &state);

    return result;

#endif
}

std::u8string sgl::ToU8String(const std::string& str)
{
	return std::u8string(str.begin(), str.end());
}

std::u8string sgl::ToU8String(const std::wstring& str)
{
    if (str.empty())
        return u8"";

#ifdef _WIN32

    int size = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);

    std::u8string result(size, 0);

    WideCharToMultiByte(
        CP_UTF8,
        0,
        str.data(),
        (int)str.size(),
        (char*)result.data(),
        size,
        nullptr,
        nullptr
    );

    return result;

#else

    std::mbstate_t state{};
    const wchar_t* src = str.data();

    size_t size = wcsrtombs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::u8string result(size, 0);

    state = std::mbstate_t{};
    src = str.data();

    wcsrtombs((char*)result.data(), &src, size, &state);

    return result;

#endif
}

std::wstring sgl::ToWString(const std::string& str)
{
    if (str.empty())
        return L"";

#ifdef _WIN32

    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);

    std::wstring result(size, 0);

    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size);

    return result;

#else

    std::mbstate_t state{};
    const char* src = str.data();

    size_t size = mbsrtowcs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::wstring result(size, 0);

    state = std::mbstate_t{};
    src = str.data();

    mbsrtowcs(result.data(), &src, size, &state);

    return result;

#endif
}

std::wstring sgl::ToWString(const std::u8string& str)
{
    if (str.empty())
        return L"";

#ifdef _WIN32

    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        (const char*)str.data(),
        (int)str.size(),
        nullptr,
        0
    );

    std::wstring result(size, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        (const char*)str.data(),
        (int)str.size(),
        result.data(),
        size
    );

    return result;

#else

    std::mbstate_t state{};
    const char* src = (const char*)str.data();

    size_t size = mbsrtowcs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::wstring result(size, 0);

    state = std::mbstate_t{};
    src = (const char*)str.data();

    mbsrtowcs(result.data(), &src, size, &state);

    return result;

#endif
}