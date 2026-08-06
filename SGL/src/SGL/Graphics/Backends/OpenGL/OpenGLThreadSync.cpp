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
std::queue<gluint> gTextureDeleteQueue;
std::queue<gluint> gFramebufferDeleteQueue;
std::queue<gluint> gBufferDeleteQueue;

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

void sgl_OpenGLThreadSync_DeleteTexture(gluint id)
{
    guard;
    gTextureDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_DeleteFramebuffer(gluint id)
{
    guard;
    gFramebufferDeleteQueue.push(id);
}

void sgl_OpenGLThreadSync_DeleteBuffer(gluint id)
{
    guard;
    gBufferDeleteQueue.push(id);
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

    while (!gTextureDeleteQueue.empty())
    {
        gluint id = gTextureDeleteQueue.front();
        gTextureDeleteQueue.pop();
        glDeleteTextures(1, &id);
    }

    while (!gFramebufferDeleteQueue.empty())
    {
        gluint id = gFramebufferDeleteQueue.front();
        gFramebufferDeleteQueue.pop();
        glDeleteFramebuffers(1, &id);
    }

    while (!gBufferDeleteQueue.empty())
    {
        gluint id = gBufferDeleteQueue.front();
        gBufferDeleteQueue.pop();
        glDeleteBuffers(1, &id);
    }
}