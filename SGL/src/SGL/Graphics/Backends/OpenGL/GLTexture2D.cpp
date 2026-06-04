#include "GLTexture2D.h"
#include <SGL/Util/Memory.h>

#define GetSelf sgl_GLTexture2D* self = (sgl_GLTexture2D*)tex

static void GLTexture_Destroy(sgl_Texture* tex)
{
    GetSelf;

    glDeleteTextures(1, &self->texture);

    sgl::Memory::Delete(self);
}

static void GLTexture_Bind(sgl_Texture* tex, uint32 unit)
{
    GetSelf;
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

    glCreateTextures(GL_TEXTURE_2D, 1, &texture->texture);

    glTextureParameteri(texture->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(texture->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureParameteri(texture->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(texture->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(texture->texture, 1, GL_RGBA8, size.width, size.height);
    glTextureSubImage2D(texture->texture, 0, 0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    return texture;
}