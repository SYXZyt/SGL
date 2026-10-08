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
    void (*SetDepthWriteEnabled)(struct sgl_GraphicsDevice* self, bool enabled);
    void (*Resize)(struct sgl_GraphicsDevice* self, sgl_Vec2i newSize);
    void (*BeginFrame)(struct sgl_GraphicsDevice* self);
    void (*EndFrame)(struct sgl_GraphicsDevice* self);
    void (*Destroy)(struct sgl_GraphicsDevice* self);
    void (*SwapBuffer)(struct sgl_GraphicsDevice* self);
    void (*Draw)(struct sgl_GraphicsDevice* self, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount);
    void (*DrawInstanced)(struct sgl_GraphicsDevice* self, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount, uint32 instanceCount);
    void (*SetVsync)(struct sgl_GraphicsDevice* self, bool enable);
    void (*SetScissor)(struct sgl_GraphicsDevice* self, bool enabled, sgl_Vec2i position, sgl_Vec2i size);

    void (*ImGui_Init)(struct sgl_GraphicsDevice* self);
    void (*ImGui_Shutdown)(struct sgl_GraphicsDevice* self);
    void (*ImGui_NewFrame)(struct sgl_GraphicsDevice* self);
    void (*ImGui_RenderDrawData)(struct sgl_GraphicsDevice* self);
} sgl_GraphicsDeviceVTable;

/// @brief An object for handling graphics devices
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
    bool depthWriteEnabled;
    bool vsync;
} sgl_GraphicsDevice;

/// @brief Create a new graphics device. This is thread unsafe and must be called on the main thread
/// @param window The window to render to
/// @return A pointer to the created device
SGL_API extern sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window);

/// @brief Destroy a graphics device
/// @param device A pointer to the device to destroy
SGL_API extern void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device);

/// @brief Change the current window clear colour
/// @param device The device to change the colour for
/// @param colour The new clear colour
SGL_API extern void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour);

/// @brief Change if depth testing is active or not. Can be useful for 2D
/// @param device The device to use
/// @param enabled true to use depth testing, otherwise false
SGL_API extern void sgl_GraphicsDevice_SetDepthTestEnabled(sgl_GraphicsDevice* device, bool enabled);

/// @brief Check if depth testing is enabled
/// @param device The device to use
/// @return True if depth testing is enabled, otherwise false
SGL_API extern bool sgl_GraphicsDevice_GetDepthTestEnabled(sgl_GraphicsDevice* device);

/// @brief Change if draws calls write to the depth buffer (This is different from SetDepthTestEnabled. This is for writes, that is for reads)
/// @param device The device to use
/// @param enabled True to write depth, otherwise false
SGL_API extern void sgl_GraphicsDevice_SetDepthWriteEnabled(sgl_GraphicsDevice* device, bool enabled);

/// @brief Check if depth writes are enabled
/// @param device The device to use
/// @return True if depth writes are enabled, otherwise false
SGL_API extern bool sgl_GraphicsDevice_GetDepthWriteEnabled(sgl_GraphicsDevice* device);

/// @brief Begin preperations for a new frame. All draw calls must be after this
/// @param device The device to use
SGL_API extern void sgl_GraphicsDevice_BeginFrame(sgl_GraphicsDevice* device);

/// @brief End a frame. All draws calls must be before this
/// @param device The device to use
SGL_API extern void sgl_GraphicsDevice_EndFrame(sgl_GraphicsDevice* device);

/// @brief Present the back buffer to the display. Should be the final call in the render loop
/// @param device The device to use
SGL_API extern void sgl_GraphicsDevice_SwapBuffer(sgl_GraphicsDevice* device);

/// @brief Draw an object to the screen
/// @param device The device to render with
/// @param va The vertices to draw
/// @param shader The shader to draw with
/// @param textures An array of textures
/// @param textureCount How many textures are in the array
/// @param buffers An array of uniform buffers
/// @param count How many uniform buffers to use
SGL_API extern void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t count);

/// @brief Draw multiple instances of an object in a single draw call. The vertex array's layout must include per-instance elements
/// @param device The device to render with
/// @param va The vertices to draw
/// @param shader The shader to draw with
/// @param textures An array of textures
/// @param textureCount How many textures are in the array
/// @param buffers An array of uniform buffers
/// @param count How many uniform buffers to use
/// @param instanceCount How many instances to draw
SGL_API extern void sgl_GraphicsDevice_DrawInstanced(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t count, uint32 instanceCount);

/// @brief Add a new post processing effect
/// @param device The device to use
/// @param effect The effect to add
SGL_API extern void sgl_GraphicsDevice_AddEffect(sgl_GraphicsDevice* device, struct sgl_PostProcess* effect);

/// @brief Get all of the loaded post processing effects
/// @param device The device to use
/// @param effects A pointer to an array of effect pointers
/// @return The number of effects loaded
SGL_API extern size_t sgl_GraphicsDevice_GetEffects(sgl_GraphicsDevice* device, struct sgl_PostProcess*** effects);

/// @brief Get the vertex layout used for rendering post processing effects
/// @param device The device to use
/// @return The vertex layout
SGL_API extern struct sgl_VertexLayout* sgl_GraphicsDevice_GetPostProcessLayout(sgl_GraphicsDevice* device);

/// @brief Set the state of vsync
/// @param device The device to use
/// @param enable True to enable vsync, false to disable
SGL_API extern void sgl_GraphicsDevice_SetVsync(sgl_GraphicsDevice* device, bool enable);

/// @brief Define a scissor region, where any pixels outside this region will not draw
/// @param device The device to use
/// @param enabled Whether to enable or disable scissor 
/// @param position The top-left screen coord
/// @param size How large the scissor rect should be
SGL_API extern void sgl_GraphicsDevice_SetScissor(sgl_GraphicsDevice* device, bool enabled, sgl_Vec2i position, sgl_Vec2i size);

SGL_API extern void sgl_GraphicsDevice_ImGui_Init(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_Shutdown(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_NewFrame(sgl_GraphicsDevice* device);
SGL_API extern void sgl_GraphicsDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* device);

SGL_END