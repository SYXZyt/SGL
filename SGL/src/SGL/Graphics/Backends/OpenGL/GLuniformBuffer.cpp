#include "GLUniformBuffer.h"
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

#define GetSelf sgl_GLUniformBuffer* self = (sgl_GLUniformBuffer*)ub

static void GLDestroy(sgl_UniformBuffer* ub)
{
    GetSelf;

    sgl_OpenGLThreadSync_DeleteBuffer(self->buffer);
    self->buffer = 0;

    sgl::Memory::Delete(self);
}

static void GLBind(sgl_UniformBuffer* ub, uint32 slot)
{
    GetSelf;
    glBindBufferBase(GL_UNIFORM_BUFFER, slot, self->buffer);
}

static void GLUpload(sgl_UniformBuffer* ub, void* data)
{
    GetSelf;
    glNamedBufferSubData(self->buffer, 0, ub->size, data);
}

static sgl_UniformBufferVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Bind = &GLBind,
    .Upload = &GLUpload,
};

sgl_GLUniformBuffer* sgl_GLUniformBuffer_New(sgl_GraphicsDevice* gpu, size_t size)
{
    sgl_GLUniformBuffer* ub = sgl::Memory::New<sgl_GLUniformBuffer>();
    ub->base.vtable = &gGLVTable;
    ub->base.size = size;

    glCreateBuffers(1, &ub->buffer);
    glNamedBufferStorage(ub->buffer, size, nullptr, GL_DYNAMIC_STORAGE_BIT);

    return ub;
}