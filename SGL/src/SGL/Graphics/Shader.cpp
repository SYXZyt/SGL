#include "Shader.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLShader.h>
#include <SGL/Graphics/Backends/DirectX/DXShader.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <SGL/Graphics/VertexLayout.h>

void sgl_Shader_Load_Source(sgl_Shader* shader, const char* vSrc, const char* fSrc) {
    shader->vtable->Load(shader, vSrc, fSrc);
}

void sgl_Shader_Load_Filename(sgl_Shader* shader, const char* vFile, const char* fFile)
{
    if (!std::filesystem::exists(vFile))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + vFile).c_str());
        return;
    }

    if (!std::filesystem::exists(fFile))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + fFile).c_str());
        return;
    }

    std::ifstream v(vFile);
    std::ifstream f(fFile);

    std::stringstream vs, fs;
    vs << v.rdbuf();
    fs << f.rdbuf();

    sgl_Shader_Load_Source(shader, vs.str().c_str(), fs.str().c_str());
}

bool sgl_Shader_Contents_Loaded(sgl_Shader* shader) {
    return shader->contentsLoaded;
}

sgl_Shader* sgl_Shader_Create(sgl_GraphicsDevice* gpu, sgl_VertexLayout* layout)
{
    sgl_Shader* shader = nullptr;
    sgl_VertexLayout* ourLayout = sgl_VertexLayout_DeepCopy(layout);

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
    {
        shader = (sgl_Shader*)sgl_GLShader_Create();
    }
    else if (gpu->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        shader = (sgl_Shader*)sgl_DXShader_Create();
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    shader->gpu = gpu;
    shader->layout = ourLayout;

    return shader;
}

void sgl_Shader_Destroy(sgl_Shader* shader)
{
    sgl_VertexLayout_Destroy(shader->layout);
    shader->vtable->Destroy(shader);
}

void sgl_Shader_Bind(sgl_Shader* shader) {
    shader->vtable->Bind(shader);
}