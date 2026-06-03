#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

typedef uint8 sgl_UniformType;
enum
{
    sgl_INT,
    sgl_FLOAT,
    sgl_VEC2,
    sgl_VEC3,
    sgl_MAT4,
};

struct sgl_UniformBuffer;
typedef struct sgl_UniformBufferVTable sgl_sealed
{
    void (*Destroy)(struct sgl_UniformBuffer* self);
    void (*Bind)(struct sgl_UniformBuffer* self, uint32 slot);
    void (*Upload)(struct sgl_UniformBuffer* self, void* ptr);

} sgl_UniformBufferVTable;

typedef struct sgl_UniformBuffer sgl_sealed
{
    sgl_UniformBufferVTable* vtable;
    sgl_GraphicsDevice* gpu;
    size_t size;
} sgl_UniformBuffer;

SGL_API extern sgl_UniformBuffer* sgl_UniformBuffer_Create(sgl_GraphicsDevice* gpu, size_t size);
SGL_API extern void sgl_UniformBuffer_Destroy(sgl_UniformBuffer* ub);

SGL_API extern void sgl_UniformBuffer_Bind(sgl_UniformBuffer* ub, uint32 slot);
SGL_API extern void sgl_UniformBuffer_Upload(sgl_UniformBuffer* ub, void* data);

SGL_END

#ifdef __cplusplus

template <typename T>
inline void sgl_UniformBuffer_Upload(sgl_UniformBuffer* ub, T& t) {
    sgl_UniformBuffer_Upload(ub, &t, sizeof(T));
}

#endif