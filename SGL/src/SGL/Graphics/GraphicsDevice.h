#pragma once
#include <SGL/SGL.h>
#include <SGL/Colour.h>
#include <SGL/Maths/Vec2.h>
#include <SDL3/SDL.h>
#include <SGL/Window.h>

SGL_BEGIN

struct sgl_VertexArray;
struct sgl_Shader;
struct sgl_UniformBuffer;
struct sgl_Texture2D;

struct sgl_GraphicsDevice;
typedef struct sgl_GraphicsDeviceVTable sgl_sealed
{
    void (*SetClearColour)(struct sgl_GraphicsDevice* self, sgl_Colour clearColour);
    void (*Resize)(struct sgl_GraphicsDevice* self, sgl_Vec2i newSize);
    void (*Clear)(struct sgl_GraphicsDevice* self);
    void (*Present)(struct sgl_GraphicsDevice* self);
    void (*Destroy)(struct sgl_GraphicsDevice* self);
    void (*Draw)(struct sgl_GraphicsDevice* self, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture2D** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount);

    void (*ImGui_Init)(struct sgl_GraphicsDevice* self);
    void (*ImGui_Shutdown)(struct sgl_GraphicsDevice* self);
    void (*ImGui_NewFrame)(struct sgl_GraphicsDevice* self);
    void (*ImGui_RenderDrawData)(struct sgl_GraphicsDevice* self);
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
SGL_API extern void sgl_GraphicsDevice_Clear(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_Present(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture2D** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t count);

SGL_API extern void sgl_GraphicsDevice_ImGui_Init(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_Shutdown(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_NewFrame(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* device);

SGL_END

#undef SGL_VTABLE_ENTRY