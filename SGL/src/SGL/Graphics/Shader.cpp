#include "Shader.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLShader.h>
#include <SGL/Graphics/Backends/DirectX/DXShader.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Util/Logger.h>
#include <slang.h>
#include <slang-com-ptr.h>

static void LogSlangDiagnostics(slang::IBlob* diagnostics)
{
    if (diagnostics && diagnostics->getBufferSize() > 0)
        sgl_LogWarning((std::string("Slang: ") + (const char*)diagnostics->getBufferPointer()).c_str());
}

static slang::IGlobalSession* GetSlangGlobalSession()
{
    static Slang::ComPtr<slang::IGlobalSession> session;
    if (!session)
        slang::createGlobalSession(session.writeRef());

    return session.get();
}

void sgl_Shader_Load_Source(sgl_Shader* shader, const char* vSrc, const char* fSrc)
{
    if (shader->data_fcode.len || shader->data_vcode.len)
        SGL_REPORT_ERROR("Shader already has already had contents assigned");

    shader->data_vcode = sgl_MakeString(vSrc);
    shader->data_fcode = sgl_MakeString(fSrc);
}

void sgl_Shader_Load_Slang_File(sgl_Shader* shader, const char* file, const char* vertexEntry, const char* fragmentEntry)
{
    if (!std::filesystem::exists(file))
    {
        SGL_REPORT_ERROR(std::format("Could not find file: {}", file).c_str());
        return;
    }

    std::ifstream in(file);

    std::stringstream iss;
    iss << in.rdbuf();

    sgl_Shader_Load_Slang_Source(shader, iss.str().c_str(), vertexEntry, fragmentEntry);
}

void sgl_Shader_Load_Slang_Source(sgl_Shader* shader, const char* slangSource, const char* vertexEntry, const char* fragmentEntry)
{
    bool isDX = shader->gpu->window->cfg.backend == sgl_Backend_DIRECTX11;

    slang::IGlobalSession* globalSession = GetSlangGlobalSession();
    if (!globalSession)
    {
        SGL_REPORT_ERROR("Failed to create Slang global session");
        return;
    }

    slang::TargetDesc target = {};
    target.format = isDX ? SLANG_HLSL : SLANG_GLSL;
    target.profile = globalSession->findProfile(isDX ? "sm_5_0" : "glsl_440");

    slang::SessionDesc sessionDesc = {};
    sessionDesc.targets = &target;
    sessionDesc.targetCount = 1;
    sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;

    Slang::ComPtr<slang::ISession> session;
    if (SLANG_FAILED(globalSession->createSession(sessionDesc, session.writeRef())))
    {
        SGL_REPORT_ERROR("Failed to create Slang session");
        return;
    }

    Slang::ComPtr<slang::IBlob> loadDiagnostics;
    slang::IModule* module = session->loadModuleFromSourceString("sgl_shader", "sgl_shader.slang", slangSource, loadDiagnostics.writeRef());
    LogSlangDiagnostics(loadDiagnostics);

    if (!module)
    {
        SGL_REPORT_ERROR("Slang failed to load shader module");
        return;
    }

    Slang::ComPtr<slang::IEntryPoint> vsEntry, fsEntry;
    module->findEntryPointByName(vertexEntry, vsEntry.writeRef());
    module->findEntryPointByName(fragmentEntry, fsEntry.writeRef());

    if (!vsEntry || !fsEntry)
    {
        SGL_REPORT_ERROR((std::string("Slang: could not find entry point(s) '") + vertexEntry + "'/'" + fragmentEntry + "'").c_str());
        return;
    }

    slang::IComponentType* components[] = { module, vsEntry.get(), fsEntry.get() };

    Slang::ComPtr<slang::IComponentType> program;
    Slang::ComPtr<slang::IBlob> compositeDiagnostics;
    session->createCompositeComponentType(components, 3, program.writeRef(), compositeDiagnostics.writeRef());
    LogSlangDiagnostics(compositeDiagnostics);

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    Slang::ComPtr<slang::IBlob> linkDiagnostics;
    program->link(linkedProgram.writeRef(), linkDiagnostics.writeRef());
    LogSlangDiagnostics(linkDiagnostics);

    if (!linkedProgram)
    {
        SGL_REPORT_ERROR("Slang failed to link shader program");
        return;
    }

    Slang::ComPtr<slang::IBlob> vsCode, fsCode, vsCodeDiag, fsCodeDiag;
    linkedProgram->getEntryPointCode(0, 0, vsCode.writeRef(), vsCodeDiag.writeRef());
    linkedProgram->getEntryPointCode(1, 0, fsCode.writeRef(), fsCodeDiag.writeRef());
    LogSlangDiagnostics(vsCodeDiag);
    LogSlangDiagnostics(fsCodeDiag);

    if (!vsCode || !fsCode)
    {
        SGL_REPORT_ERROR("Slang failed to generate target code");
        return;
    }

    if (isDX)
    {
        strncpy_s(shader->vertexEntryName, vertexEntry, sizeof(shader->vertexEntryName) - 1);
        strncpy_s(shader->fragmentEntryName, fragmentEntry, sizeof(shader->fragmentEntryName) - 1);
    }

    sgl_Shader_Load_Source(shader, (const char*)vsCode->getBufferPointer(), (const char*)fsCode->getBufferPointer());
}

void sgl_Shader_Load_File(sgl_Shader* shader, const char* vFile, const char* fFile)
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

    strcpy_s(shader->vertexEntryName, "main");
    strcpy_s(shader->fragmentEntryName, "main");

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