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

/// @brief A shader
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

/// @brief Create a new shader
/// @param gpu The device to use
/// @param layout The vertex layout
/// @return A new blank shader
SGL_API extern sgl_Shader* sgl_Shader_Create(struct sgl_GraphicsDevice* gpu, struct sgl_VertexLayout* layout);

/// @brief Load a shader by source using the API specific language (GLSL/HLSL)
/// @param shader The shader to load into
/// @param vSrc Vertex shader source
/// @param fSrc Fragment shader source
SGL_API extern void sgl_Shader_Load_Source(sgl_Shader* shader, const char* vSrc, const char* fSrc);

/// @brief Load a shader from two files. Expected to be the API specific languge (GLSL/HLSL)
/// @param shader The shader to load into
/// @param vFile The name of the vertex file
/// @param fFile The name of the fragment file
SGL_API extern void sgl_Shader_Load_File(sgl_Shader* shader, const char* vFile, const char* fFile);

/// @brief Load a shader from a file using Slang
/// @param shader The shader to load into
/// @param file The name of the .slang file
/// @param vertexEntry The name of the vertex entry point
/// @param fragmentEntry The name of the fragment entry point
SGL_API extern void sgl_Shader_Load_Slang_File(sgl_Shader* shader, const char* file, const char* vertexEntry, const char* fragmentEntry);

/// @brief Load a shader from source using Slang
/// @param shader The shader to load into
/// @param slangSource The shader source code
/// @param vertexEntry The name of the vertex entry point
/// @param fragmentEntry The name of the fragment entry point
SGL_API extern void sgl_Shader_Load_Slang_Source(sgl_Shader* shader, const char* slangSource, const char* vertexEntry, const char* fragmentEntry);

SGL_API extern bool sgl_Shader_Contents_Loaded(sgl_Shader* shader);

/// @brief Destroy a shader
/// @param shader The shader to destroy
SGL_API extern void sgl_Shader_Destroy(sgl_Shader* shader);

/// @brief Bind a shader for use
/// @param shader The shader to bind
SGL_API extern void sgl_Shader_Bind(sgl_Shader* shader);

SGL_END