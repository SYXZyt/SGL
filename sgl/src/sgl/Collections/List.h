#pragma once
#include <sgl/Memory/StlAllocator.h>
#include <vector>

namespace sgl
{
    template <typename T>
    using List = std::vector<T, StlAllocator<T>>;
}