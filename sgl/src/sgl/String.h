#pragma once
#include <sgl/sgl.h>
#include <string>
#include <type_traits>
#include <format>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace sgl
{
    using String = std::u8string;

    extern SGL_API std::string ToString(const std::u8string& str);
    extern SGL_API std::string ToString(const std::wstring& str);

    extern SGL_API std::u8string ToU8String(const std::string& str);
    extern SGL_API std::u8string ToU8String(const std::wstring& str);

    extern SGL_API std::wstring ToWString(const std::string& str);
    extern SGL_API std::wstring ToWString(const std::u8string& str);

#ifdef _WIN32
    extern SGL_API const char* HResultToString(HRESULT hr);
#endif

    template <typename From, typename To>
    struct StringConverter;

    template <typename T>
    struct StringConverter<T, T> {
        static T Convert(const T& t) {
            return t;
        }
    };

#pragma region To_String
	template <>
	struct StringConverter<std::u8string, std::string> {
		static std::string Convert(const std::u8string& str) {
			return ToString(str);
		}
	};

	template <>
	struct StringConverter<std::wstring, std::string> {
		static std::string Convert(const std::wstring& str) {
			return ToString(str);
		}
	};

	template <>
	struct StringConverter<const char*, std::string> {
		static std::string Convert(const char* str) {
			return std::string(str);
		}
	};

	template <>
	struct StringConverter<const wchar_t*, std::string> {
		static std::string Convert(const wchar_t* str) {
			return ToString(std::wstring(str));
		}
	};

	template <>
	struct StringConverter<const char8_t*, std::string> {
		static std::string Convert(const char8_t* str) {
			return ToString(std::u8string(str));
		}
	};

#pragma endregion // To_String

#pragma region To_U8String	

	template <>
	struct StringConverter<std::string, std::u8string> {
		static std::u8string Convert(const std::string& str) {
			return ToU8String(str);
		}
	};

	template <>
	struct StringConverter<std::wstring, std::u8string> {
		static std::u8string Convert(const std::wstring& str) {
			return ToU8String(str);
		}
	};

	template <>
	struct StringConverter<const char*, std::u8string> {
		static std::u8string Convert(const char* str) {
			return ToU8String(std::string(str));
		}
	};

	template <>
	struct StringConverter<const wchar_t*, std::u8string> {
		static std::u8string Convert(const wchar_t* str) {
			return ToU8String(std::wstring(str));
		}
	};

	template <>
	struct StringConverter<const char8_t*, std::u8string> {
		static std::u8string Convert(const char8_t* str) {
			return std::u8string(str);
		}
	};

#pragma endregion // To_U8String

#pragma region To_WString

	template <>
	struct StringConverter<std::string, std::wstring> {
		static std::wstring Convert(const std::string& str) {
			return ToWString(str);
		}
	};

	template <>
	struct StringConverter<std::u8string, std::wstring> {
		static std::wstring Convert(const std::u8string& str) {
			return ToWString(str);
		}
	};

	template <>
	struct StringConverter<const char*, std::wstring> {
		static std::wstring Convert(const char* str) {
			return ToWString(std::string(str));
		}
	};

	template <>
	struct StringConverter<const wchar_t*, std::wstring> {
		static std::wstring Convert(const wchar_t* str) {
			return std::wstring(str);
		}
	};

	template <>
	struct StringConverter<const char8_t*, std::wstring> {
		static std::wstring Convert(const char8_t* str) {
			return ToWString(std::u8string(str));
		}
	};

#pragma endregion // To_WString

	template <typename From, typename To>
	To ConvertString(const From& str) {
		return StringConverter<From, To>::Convert(str);
	}

	template<typename... Args>
	inline std::u8string FormatString(const char8_t* format, Args&&... args)
	{
		const char* fmt = (const char*)format;
		std::string tmp = std::vformat(fmt, std::make_format_args(args...));

		return std::u8string((const char8_t*)tmp.data(), tmp.size());
	}

	template <typename T>
	inline String ToString(const T& t) {
		return ConvertString<std::string, String>(std::to_string(t));
	}
}