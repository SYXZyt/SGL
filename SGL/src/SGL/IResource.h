#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef uint8 sgl_InitState;
enum
{
    /// @brief Data for this object is loading. Do not try to use in this state. Wait.
    sgl_InitState_LOADING,

    /// @brief Data for the object has been loaded. It needs initialising before use.
    sgl_InitState_LOADED,

    /// @brief Object is loaded and initialised and is ready for use
    sgl_InitState_INITIALISED,
};

typedef struct sgl_IResource sgl_sealed
{
    sgl_InitState initState;
    bool isInitialised;
} sgl_IResource;

SGL_END