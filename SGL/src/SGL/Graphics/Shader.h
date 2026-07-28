#pragma once
#include <SGL/SGL.h>
#include <SGL/Util/String.h>

SGL_BEGIN

struct sgl_VertexLayout;
struct sgl_GraphicsDevice;
struct sgl_Shader;
typedef struct sgl_ShaderVTable sgl_sealed
{
    void (*Bind)(struct sgl_Shader* self);
    void (*Destroy)(struct sgl_Shader* self);
    void (*Load)(struct sgl_Shader* self, const char* vSrc, const char* fSrc);
} sgl_ShaderVTable;

typedef struct sgl_Shader sgl_sealed
{
    const sgl_ShaderVTable* vtable;
    sgl_String data_vcode;
    sgl_String data_fcode;
    struct sgl_GraphicsDevice* gpu;
    struct sgl_VertexLayout* layout;
    bool contentsLoaded;

    char vertexEntryName[64];
    char fragmentEntryName[64];
} sgl_Shader;

SGL_API extern sgl_Shader* sgl_Shader_Create(struct sgl_GraphicsDevice* gpu, struct sgl_VertexLayout* layout);
SGL_API extern void sgl_Shader_Load_Source(sgl_Shader* shader, const char* vSrc, const char* fSrc);
SGL_API extern void sgl_Shader_Load_Filename(sgl_Shader* shader, const char* vFile, const char* fFile);

SGL_API extern void sgl_Shader_Load_Slang_File(sgl_Shader* shader, const char* file, const char* vertexEntry, const char* fragmentEntry);
SGL_API extern void sgl_Shader_Load_Slang_Source(sgl_Shader* shader, const char* slangSource, const char* vertexEntry, const char* fragmentEntry);

SGL_API extern bool sgl_Shader_Contents_Loaded(sgl_Shader* shader);

SGL_API extern void sgl_Shader_Destroy(sgl_Shader* shader);

SGL_API extern void sgl_Shader_Bind(sgl_Shader* shader);

SGL_END