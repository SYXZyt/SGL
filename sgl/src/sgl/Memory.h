#pragma once
#include <sgl/sgl.h>
#include <new>
#include <typeinfo>
#include <utility>

#define SGL_ALLOCATOR_FRIEND

#ifdef _DEBUG
#define SGL_DISALLOW_UNOWNED_POINTERS
#define SGL_MEMORY_TRACK
#endif