#pragma once
#include <SGL/SGL.h>
#include <SGL/Colour.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

struct sgl_Texture;
struct sgl_Framebuffer;

typedef struct sgl_FramebufferVTable sgl_sealed
{
    void (*Destroy)(struct sgl_Framebuffer* self);
    void (*Resize)(struct sgl_Framebuffer* self, sgl_Vec2i size);
    void (*Bind)(struct sgl_Framebuffer* self);
    void (*Clear)(struct sgl_Framebuffer* self, sgl_Colour colour);
} sgl_FramebufferVTable;

/// @brief A framebuffer which can be rendered to
typedef struct sgl_Framebuffer sgl_sealed
{
    const sgl_FramebufferVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_Vec2i size;
    bool hasDepth;

    struct sgl_Texture* colourTexture;
} sgl_Framebuffer;

/// @brief Create a new framebuffer
/// @param device The device to own the  buffer
/// @param size The size of the framebuffer
/// @param hasDepth Does the framebuffer have a depth buffer
/// @return 
SGL_API extern sgl_Framebuffer* sgl_Framebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth);

/// @brief Destroy a framebuffer
/// @param framebuffer The buffer to destroy
SGL_API extern void sgl_Framebuffer_Destroy(sgl_Framebuffer* framebuffer);

/// @brief Resize a framebuffer
/// @param framebuffer The buffer to resize
/// @param size The new size
SGL_API extern void sgl_Framebuffer_Resize(sgl_Framebuffer* framebuffer, sgl_Vec2i size);

/// @brief Bind a framebuffer for rendering to
/// @param framebuffer The buffer to bind
SGL_API extern void sgl_Framebuffer_Bind(sgl_Framebuffer* framebuffer);

/// @brief Clear a framebuffer
/// @param framebuffer The buffer to clear
/// @param colour The colour to write
SGL_API extern void sgl_Framebuffer_Clear(sgl_Framebuffer* framebuffer, sgl_Colour colour);

/// @brief Get the texture the buffer writes
/// @param framebuffer The buffer to get the texture of
/// @return The texture containing the buffer contents. This is owned by the buffer. Do not delete
SGL_API extern struct sgl_Texture* sgl_Framebuffer_GetTexture(sgl_Framebuffer* framebuffer);

/// @brief Get the size of a framebuffer
/// @param framebuffer The buffer to get the size of
/// @return The size of the buffer in pixels
SGL_API extern sgl_Vec2i sgl_Framebuffer_GetSize(sgl_Framebuffer* framebuffer);

SGL_END