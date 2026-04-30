#pragma once
#include <sgl/Memory/Memory.h>

namespace sgl
{
    template <typename T>
    class StlAllocator
    {
    public:
        using value_type = T;

        static T* allocator(std::size_t n)
        {
            if (n > (std::size_t)-1 / sizeof(T))
                return nullptr;

            return (T*)Memory::Malloc(n * sizeof(T));
        }

        static void deallocate(T* ptr, std::size_t) noexcept {
            Memory::Free(ptr);
        }

        template <typename U>
        bool operator==(const StlAllocator<U>&) const noexcept {
            return true;
        }

        template <typename U>
        bool operator!=(const StlAllocator<U>&) const noexcept {
            return false;
        }

        template <typename U>
        constexpr explicit StlAllocator(const StlAllocator<U>&) noexcept {}

        StlAllocator() = default;
    };
}