#pragma once
#include <sgl/sgl.h>

namespace sgl::OpenGLThreadSync
{
    extern void DeleteVAO(gluint id);
    extern void DeleteVBO(gluint id);
    extern void DeleteEBO(gluint id);
    extern void DeleteShader(gluint id);

    extern void Update();
}