#include "GLTexture2D.h"
#include <SGL/Util/Memory.h>
#include <stb/stb_image.h>

#define GetSelf sgl_GLTexture2D* self = (sgl_GLTexture2D*)tex

static void GLTexture_Destroy(sgl_Texture* tex)
{
    GetSelf;

    if (tex->gpuLoaded)
        glDeleteTextures(1, &self->texture);

    if (self->base.pixels)
        stbi_image_free(self->base.pixels);

    sgl::Memory::Delete(self);
}

static void GLEnsureGPUResources(sgl_GLTexture2D* self)
{
    sgl_Texture* tex = &self->base.base;

    glCreateTextures(GL_TEXTURE_2D, 1, &self->texture);

    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureParameteri(self->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(self->texture, 1, GL_RGBA8, tex->size.width, tex->size.height);
    glTextureSubImage2D(self->texture, 0, 0, 0, tex->size.width, tex->size.height, GL_RGBA, GL_UNSIGNED_BYTE, self->base.pixels);

    glGenerateTextureMipmap(self->texture);

    stbi_image_free(self->base.pixels);
    self->base.pixels = nullptr;

    tex->gpuLoaded = true;
}

static void GLTexture_Bind(sgl_Texture* tex, uint32 unit)
{
    GetSelf;

    if (!tex->gpuLoaded)
        GLEnsureGPUResources(self);

    glBindTextureUnit(unit, self->texture);
}

static sgl_TextureVTable gGLVTable =
{
    .Destroy = &GLTexture_Destroy,
    .Bind = &GLTexture_Bind,
};

sgl_GLTexture2D* sgl_GLTexture2D_Create(void* data, sgl_Vec2i size)
{
    sgl_GLTexture2D* texture = sgl::Memory::New<sgl_GLTexture2D>();
    texture->base.base.size = size;
    texture->base.base.vtable = &gGLVTable;
    texture->base.base.gpuLoaded = false;
    texture->base.pixels = data;
    texture->texture = 0;

    return texture;
}