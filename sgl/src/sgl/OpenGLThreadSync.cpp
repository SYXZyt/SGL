#include "OpenGLThreadSync.h"
#include <glad/glad.h>
#include <queue>
#include <mutex>

#define guard do { std::lock_guard lock(gDeleteMutex); } while (0)

std::mutex gDeleteMutex;

std::queue<gluint> gVaoDeleteQueue;
std::queue<gluint> gVboDeleteQueue;
std::queue<gluint> gEboDeleteQueue;
std::queue<gluint> gShaderDeleteQueue;

void sgl::OpenGLThreadSync::DeleteVAO(gluint id)
{
    guard;
    gVaoDeleteQueue.push(id);
}

void sgl::OpenGLThreadSync::DeleteVBO(gluint id)
{
    guard;
    gVboDeleteQueue.push(id);
}

void sgl::OpenGLThreadSync::DeleteEBO(gluint id)
{
    guard;
    gEboDeleteQueue.push(id);
}

void sgl::OpenGLThreadSync::DeleteShader(gluint id)
{
    guard;
    gShaderDeleteQueue.push(id);
}

void sgl::OpenGLThreadSync::Update()
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