#include "OpenGLThreadSync.h"
#include <glad/glad.h>
#include <mutex>
#include <queue>

#define guard std::lock_guard lock(gDeleteMutex)

std::mutex gDeleteMutex;

std::queue<gluint> gVaoDeleteQueue;
std::queue<gluint> gVboDeleteQueue;
std::queue<gluint> gEboDeleteQueue;
std::queue<gluint> gShaderDeleteQueue;

void sgl_OpenGLThreadSync_DeleteVAO(gluint id)
{
    guard;
    gVaoDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_DeleteVBO(gluint id)
{
    guard;
    gVboDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_DeleteEBO(gluint id)
{
    guard;
    gEboDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_DeleteShader(gluint id)
{
    guard;
    gShaderDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_Update()
{
    guard;

    while (!gVaoDeleteQueue.empty())
    {
        gluint id = gVaoDeleteQueue.front();
        gVaoDeleteQueue.pop();
        glDeleteVertexArrays(1, &id);
    }

    while (!gVboDeleteQueue.empty())
    {
        gluint id = gVboDeleteQueue.front();
        gVboDeleteQueue.pop();
        glDeleteBuffers(1, &id);
    }

    while (!gEboDeleteQueue.empty())
    {
        gluint id = gEboDeleteQueue.front();
        gEboDeleteQueue.pop();
        glDeleteBuffers(1, &id);
    }

    while (!gShaderDeleteQueue.empty())
    {
        gluint id = gShaderDeleteQueue.front();
        gShaderDeleteQueue.pop();
        glDeleteShader(id);
    }
}