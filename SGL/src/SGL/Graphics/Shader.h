#pragma once
#include <SGL/SGL.h>
#include <SGL/Util/String.h>
#include <SGL/IResource.h>

SGL_BEGIN

struct sgl_GraphicsDevice;
struct sgl_Shader;
typedef struct sgl_ShaderVTable sgl_sealed
{
    void (*Initialise)(struct sgl_Shader* self);
    void (*Bind)(struct sgl_Shader* self);
    void (*Destroy)(struct sgl_Shader* self);
} sgl_ShaderVTable;

typedef struct sgl_Shader sgl_sealed
{
    sgl_IResource resource;
    const sgl_ShaderVTable* vtable;
    sgl_String data_vcode;
    sgl_String data_fcode;
    struct sgl_GraphicsDevice* gpu;
} sgl_Shader;

SGL_API extern sgl_Shader* sgl_Shader_Create_Source(struct sgl_GraphicsDevice* gpu, const char* vSrc, const char* fSrc);
SGL_API extern sgl_Shader* sgl_Shader_Create_Filename(struct sgl_GraphicsDevice* gpu, const char* vFile, const char* fFile);

SGL_API extern void sgl_Shader_Destroy(sgl_Shader* shader);

SGL_API extern void sgl_Shader_Bind(sgl_Shader* shader);

SGL_END