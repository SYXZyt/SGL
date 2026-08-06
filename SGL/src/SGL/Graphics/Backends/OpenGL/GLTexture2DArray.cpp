#include "GLTexture2DArray.h"
#include <SGL/Util/Memory.h>
#include <stb/stb_image.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

#define GetSelf sgl_GLTexture2DArray* self = (sgl_GLTexture2DArray*)tex

static void GLDestroy(sgl_Texture* tex)
{
    GetSelf;

    if (tex->gpuLoaded)
        sgl_OpenGLThreadSync_DeleteTexture(self->texture);

    if (self->base.pixels)
        stbi_image_free(self->base.pixels);

    sgl::Memory::Delete(self);
}

static void GLEnsureGPUResources(sgl_GLTexture2DArray* self)
{
    sgl_Texture* tex = &self->base.base;
    sgl_Vec2i atlasSize = tex->size;
    sgl_Vec2i frameSize = self->base.frameSize;
    uint32 sliceCount = self->base.textureCount;

    uint32 cols = atlasSize.width / frameSize.width;
    uint32 rows = atlasSize.height / frameSize.height;

    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &self->texture);
    glTextureStorage3D(
        self->texture,
        1,
        GL_RGBA8,
        (GLsizei)frameSize.width,
        (GLsizei)frameSize.height,
        (GLsizei)sliceCount
    );

    uint32 sliceIndex = 0;

    byte* bytes = (byte*)self->base.pixels;
    for (uint32 y = 0; y < rows; ++y)
    {
        for (uint32 x = 0; x < cols; ++x)
        {
            for (uint32 row = 0; row < (uint32)frameSize.height; ++row)
            {
                const byte* src = bytes + ((y * frameSize.height + row) * atlasSize.width + x * frameSize.width) * 4;

                glTextureSubImage3D(
                    self->texture,
                    0,
                    0,
                    (GLint)row,
                    (GLint)sliceIndex,
                    (GLint)frameSize.width,
                    1,
                    1,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    src
                );
            }

            ++sliceIndex;
        }
    }

    glTextureParameteri(self->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateTextureMipmap(self->texture);

    stbi_image_free(self->base.pixels);
    self->base.pixels = nullptr;

    tex->gpuLoaded = true;
}

static void GLBind(sgl_Texture* tex, uint32 unit)
{
    GetSelf;

    if (!tex->gpuLoaded)
        GLEnsureGPUResources(self);

    glBindTextureUnit(unit, self->texture);
}

static sgl_TextureVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Bind = &GLBind,
};

sgl_GLTexture2DArray* sgl_GLTexture2DArray_Create(void* data, sgl_Vec2i atlasSize, sgl_Vec2i frameSize)
{
    sgl_GLTexture2DArray* texture = sgl::Memory::New<sgl_GLTexture2DArray>();
    texture->base.base.size = atlasSize;
    texture->base.frameSize = frameSize;
    texture->base.base.vtable = &gGLVTable;
    texture->base.base.gpuLoaded = false;
    texture->base.pixels = data;

    uint32 cols = atlasSize.width / frameSize.width;
    uint32 rows = atlasSize.height / frameSize.height;
    texture->base.textureCount = cols * rows;

    texture->texture = 0;

    return texture;
}