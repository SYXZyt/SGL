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
struct sgl_Texture;
struct sgl_PostProcess;

struct sgl_GraphicsDevice;
typedef struct sgl_GraphicsDeviceVTable sgl_sealed
{
    void (*SetClearColour)(struct sgl_GraphicsDevice* self, sgl_Colour clearColour);
    void (*SetDepthTestEnabled)(struct sgl_GraphicsDevice* self, bool enabled);
    void (*Resize)(struct sgl_GraphicsDevice* self, sgl_Vec2i newSize);
    void (*BeginFrame)(struct sgl_GraphicsDevice* self);
    void (*EndFrame)(struct sgl_GraphicsDevice* self);
    void (*Destroy)(struct sgl_GraphicsDevice* self);
    void (*SwapBuffer)(struct sgl_GraphicsDevice* self);
    void (*Draw)(struct sgl_GraphicsDevice* self, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount);

    void (*ImGui_Init)(struct sgl_GraphicsDevice* self);
    void (*ImGui_Shutdown)(struct sgl_GraphicsDevice* self);
    void (*ImGui_NewFrame)(struct sgl_GraphicsDevice* self);
    void (*ImGui_RenderDrawData)(struct sgl_GraphicsDevice* self);
} sgl_GraphicsDeviceVTable;

typedef struct sgl_GraphicsDevice sgl_sealed
{
    const sgl_GraphicsDeviceVTable* vtable;
    void* vecPtr; // pointer to std::vector<sgl_PostProcess*>. Can't be bothered to implement my own vec system
    sgl_Window* window;
    struct sgl_VertexArray* screenQuad;
    struct sgl_VertexLayout* screenQuadLayout;
    sgl_Colour clearColour;
    uint32 width;
    uint32 height;
    bool depthTestEnabled;
} sgl_GraphicsDevice;

SGL_API extern sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window);
SGL_API extern void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device);

SGL_API extern void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour);

SGL_API extern void sgl_GraphicsDevice_SetDepthTestEnabled(sgl_GraphicsDevice* device, bool enabled);
SGL_API extern bool sgl_GraphicsDevice_GetDepthTestEnabled(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_BeginFrame(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_EndFrame(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_SwapBuffer(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t count);

SGL_API extern void sgl_GraphicsDevice_AddEffect(sgl_GraphicsDevice* device, struct sgl_PostProcess* effect);
SGL_API extern size_t sgl_GraphicsDevice_GetEffects(sgl_GraphicsDevice* device, struct sgl_PostProcess*** effects);

SGL_API extern struct sgl_VertexLayout* sgl_GraphicsDevice_GetPostProcessLayout(sgl_GraphicsDevice* device);

SGL_API extern void sgl_GraphicsDevice_ImGui_Init(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_Shutdown(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_NewFrame(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* device);

SGL_END