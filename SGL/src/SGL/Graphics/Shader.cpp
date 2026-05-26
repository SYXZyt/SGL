#include "Shader.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLShader.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <filesystem>
#include <fstream>
#include <sstream>

sgl_Shader* sgl_Shader_Create_Source(struct sgl_GraphicsDevice* gpu, const char* vSrc, const char* fSrc)
{
    sgl_Shader* shader = nullptr;

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
    {
        shader = (sgl_Shader*)sgl_GLShader_Create(vSrc, fSrc);
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    shader->gpu = gpu;
    return shader;
}

sgl_Shader* sgl_Shader_Create_Filename(sgl_GraphicsDevice* gpu, const char* vFile, const char* fFile)
{
    if (!std::filesystem::exists(vFile))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + vFile).c_str());
        return nullptr;
    }

    if (!std::filesystem::exists(fFile))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + fFile).c_str());
        return nullptr;
    }

    std::ifstream v(vFile);
    std::ifstream f(fFile);

    std::stringstream vs, fs;
    vs << v.rdbuf();
    fs << f.rdbuf();

    return sgl_Shader_Create_Source(gpu, vs.str().c_str(), fs.str().c_str());
}

void sgl_Shader_Destroy(sgl_Shader* shader) {
    shader->vtable->Destroy(shader);
}

void sgl_Shader_Bind(sgl_Shader* shader) {
    shader->vtable->Bind(shader);
}