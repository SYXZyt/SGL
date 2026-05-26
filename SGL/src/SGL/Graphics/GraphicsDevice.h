#pragma once
#include <SGL/SGL.h>
#include <SGL/Colour.h>
#include <SGL/Maths/Vec2.h>
#include <SDL3/SDL.h>
#include <SGL/Window.h>

SGL_BEGIN

typedef uint8 sgl_ClearFlags;
enum
{
    sgl_ClearFlag_NONE    SGL_ENUM_FLAG(0),
    sgl_ClearFlag_COLOUR  SGL_ENUM_FLAG(1),
    sgl_ClearFlag_DEPTH   SGL_ENUM_FLAG(2),
};

struct sgl_GraphicsDevice;
typedef struct sgl_GraphicsDeviceVTable sgl_sealed
{
    void (*SetClearColour)(struct sgl_GraphicsDevice* self, sgl_Colour clearColour);
    void (*Resize)(struct sgl_GraphicsDevice* self, sgl_Vec2i newSize);
    void (*Clear)(struct sgl_GraphicsDevice* self, sgl_ClearFlags clearFlags);
    void (*Present)(struct sgl_GraphicsDevice* self);
    void (*Destroy)(struct sgl_GraphicsDevice* self);
} sgl_GraphicsDeviceVTable;

typedef struct sgl_GraphicsDevice sgl_sealed
{
    const sgl_GraphicsDeviceVTable* vtable;
    sgl_Colour clearColour;
    sgl_Window* window;
    uint32 width;
    uint32 height;
} sgl_GraphicsDevice;

SGL_API extern sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window);
SGL_API extern void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device);

SGL_API extern void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour);
SGL_API extern void sgl_GraphicsDevice_Clear(sgl_GraphicsDevice* device, sgl_ClearFlags clearFlags);
SGL_API extern void sgl_GraphicsDevice_Present(sgl_GraphicsDevice* device);

SGL_END

#undef SGL_VTABLE_ENTRY