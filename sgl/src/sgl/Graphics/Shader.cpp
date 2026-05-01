#include "Shader.h"
#include <glad/glad.h>
#include <sgl/Error.h>
#include <sgl/OpenGLThreadSync.h>

void sgl::Graphics::Shader::CreateShader(const String& vcode, const String& fcode)
{
    mShaderData.vcode = vcode;
    mShaderData.fcode = fcode;
}

void sgl::Graphics::Shader::Initialise()
{
    if (IsInitialised())
        return;

    std::string vstr = ConvertString<String, std::string>(mShaderData.vcode);
    std::string fstr = ConvertString<String, std::string>(mShaderData.fcode);

    const char* v = vstr.c_str();
    const char* f = fstr.c_str();

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
        SGL_REPORT_ERROR(FormatString(u8"Vertex shader failed to compile:\n{}", infoLog));
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
        SGL_REPORT_ERROR(FormatString(u8"Vertex shader failed to compile:\n{}", infoLog));
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    mShaderId = glCreateProgram();
    glAttachShader(mShaderId, vertex);
    glAttachShader(mShaderId, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    mInitState = InitState::INITIALISED;

    // Doing it this way will fully free the memory and reset our capacity back to 1?
    mShaderData.vcode = String();
    mShaderData.fcode = String();
}

void sgl::Graphics::Shader::Bind()
{
    if (CanInit())
        Initialise();

    glUseProgram(mShaderId);
}

sgl::Graphics::Shader::~Shader() {
    OpenGLThreadSync::DeleteShader(mShaderId);
}