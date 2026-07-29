#include "GLShader.h"
#include <SGL/Util/Memory.h>
#include <glad/glad.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

#define GetSelf sgl_GLShader* self = (sgl_GLShader*)shr;

static void GLInitialise(sgl_Shader* shr)
{
    GetSelf;

    const char* v = shr->data_vcode.str;
    const char* f = shr->data_fcode.str;

    gluint vertex, fragment;
    int success;
    char infoLog[512]{};

    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex, 1, &v, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::string message = std::string("Vertex shader failed to compiled:\n") + infoLog;
        SGL_REPORT_ERROR(message.c_str());
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    glShaderSource(fragment, 1, &f, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::string message = std::string("Fragment shader failed to compiled:\n") + infoLog;
        SGL_REPORT_ERROR(message.c_str());
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    self->shaderId = glCreateProgram();
    glAttachShader(self->shaderId, vertex);
    glAttachShader(self->shaderId, fragment);

    glLinkProgram(self->shaderId);
    glValidateProgram(self->shaderId);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    self->base.contentsLoaded = true;

    sgl_FreeString(shr->data_vcode);
    sgl_FreeString(shr->data_fcode);
}

static void GLBind(sgl_Shader* shr)
{
    GetSelf;
    glUseProgram(self->shaderId);
}

static void GLDestroy(sgl_Shader* shr)
{
    GetSelf;
    sgl_OpenGLThreadSync_DeleteShader(self->shaderId);
    sgl::Memory::Delete(self);
}

/// @todo Move the string set to be in the base, and then Load can be a one line call to Init (or put Init into the vtable)
static void GLLoad(sgl_Shader* shr, const char* vSrc, const char* fSrc)
{
    if (shr->contentsLoaded)
    {
        SGL_REPORT_ERROR("Shader already loaded");
        return;
    }

    shr->data_vcode = sgl_MakeString(vSrc);
    shr->data_fcode = sgl_MakeString(fSrc);

    GLInitialise(shr);
}

static const sgl_ShaderVTable gGLVTable =
{
    .Bind = &GLBind,
    .Destroy = &GLDestroy,
    .Load = &GLLoad,
};

sgl_GLShader* sgl_GLShader_Create()
{
    sgl_GLShader* shader = sgl::Memory::New<sgl_GLShader>();

    shader->base.contentsLoaded = false;
    shader->base.vtable = &gGLVTable;

    return shader;
}