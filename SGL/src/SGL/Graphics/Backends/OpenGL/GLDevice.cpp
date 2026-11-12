#include "GLDevice.h"
#include <SGL/Version.h>
#include <SGL/Util/Memory.h>
#include <glad/glad.h>
#include <sstream>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <SGL/Graphics/Framebuffer.h>
#include <SGL/Graphics/PostProcess.h>
#include <SGL/Graphics/Backends/OpenGL/GLFramebuffer.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

template <typename T>
T* GetBackend(void* ptr) {
    return reinterpret_cast<T*>(ptr);
}

#define GetSelf sgl_GLDevice* self = (sgl_GLDevice*)dev

static void GLDevice_SetClearColour(sgl_GraphicsDevice* dev, sgl_Colour colour)
{
    dev->clearColour = colour;
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

static void GLDevice_SetDepthTestEnabled(sgl_GraphicsDevice* dev, bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

static void GLDevice_SetDepthWriteEnabled(sgl_GraphicsDevice* dev, bool enabled) {
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}

static void GLDevice_Resize(sgl_GraphicsDevice* dev, sgl_Vec2i newSize)
{
    if (newSize.width <= 0 || newSize.height <= 0)
        return;

    dev->width = newSize.width;
    dev->height = newSize.height;

    glViewport(0, 0, dev->width, dev->height);

    int scale = dev->sceneScale < 1 ? 1 : dev->sceneScale;
    uint32 uscale = (uint32)scale;
    uint32 w = (dev->width + uscale - 1) / uscale;
    uint32 h = (dev->height + uscale - 1) / uscale;
    w += (w & 1);
    h += (h & 1);

    sgl_Framebuffer_Resize(dev->sceneFramebuffer, { (int)w, (int)h });
}

static void GLDevice_SetBlendMode(sgl_GraphicsDevice* dev, sgl_BlendMode mode)
{
    if (mode == sgl_BlendMode_OPAQUE)
    {
        glDisable(GL_BLEND);
        return;
    }

    glEnable(GL_BLEND);

    switch (mode)
    {
        case sgl_BlendMode_ADDITIVE:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case sgl_BlendMode_MULTIPLY:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case sgl_BlendMode_MAX:
            glBlendEquation(GL_MAX);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case sgl_BlendMode_ALPHA:
        default:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            break;
    }
}

static void GLDevice_BindBackbuffer(sgl_GraphicsDevice* dev) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void GLDevice_BeginFrame(sgl_GraphicsDevice* dev)
{
    sgl_Framebuffer_Bind(dev->sceneFramebuffer);
    sgl_Framebuffer_Clear(dev->sceneFramebuffer, dev->clearColour);

    dev->blendMode = sgl_BlendMode_ALPHA;
    GLDevice_SetBlendMode(dev, sgl_BlendMode_ALPHA);
}

static void GLDevice_EndFrame(sgl_GraphicsDevice* dev)
{
    sgl_PostProcess** effects;
    size_t effectCount = sgl_GraphicsDevice_GetEffects(dev, &effects);

    sgl_Framebuffer* currentFramebuffer = dev->sceneFramebuffer;
    bool ranEffect = false;

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    dev->blendMode = sgl_BlendMode_ALPHA;
    GLDevice_SetBlendMode(dev, sgl_BlendMode_ALPHA);

    glViewport(0, 0, (GLsizei)dev->width, (GLsizei)dev->height);

    for (size_t i = 0; i < effectCount; ++i)
    {
        sgl_PostProcess* effect = effects[i];

        if (!sgl_PostProcess_GetEnabled(effect))
            continue;

        sgl_PostProcess_Bind(effect);
        sgl_VertexArray_Bind(dev->screenQuad);
        sgl_Texture_Bind(sgl_Framebuffer_GetTexture(currentFramebuffer), 0);

        glDrawArrays(GL_TRIANGLES, 0, dev->screenQuad->vertexCount);

        currentFramebuffer = sgl_PostProcess_GetTarget(effect);
        ranEffect = true;
    }

    sgl_Vec2i sourceSize = sgl_Framebuffer_GetSize(currentFramebuffer);
    gluint sourceFbo = ((sgl_GLFramebuffer*)currentFramebuffer)->fbo;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    int destX0 = 0, destY0 = 0, destX1 = dev->width, destY1 = dev->height;

    if (!ranEffect)
    {
        int scale = dev->sceneScale < 1 ? 1 : dev->sceneScale;
        int scaledWidth = sourceSize.width * scale;
        int scaledHeight = sourceSize.height * scale;

        destX0 = (dev->width - scaledWidth) / 2;
        destY0 = (dev->height - scaledHeight) / 2;
        destX1 = destX0 + scaledWidth;
        destY1 = destY0 + scaledHeight;
    }

    glBlitFramebuffer(
        0, 0,
        sourceSize.width, sourceSize.height,
        destX0, destY0,
        destX1, destY1,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_CULL_FACE);

    if (dev->depthTestEnabled)
        glEnable(GL_DEPTH_TEST);

    sgl_OpenGLThreadSync_Update();
}

static void GLDevice_SwapBuffer(sgl_GraphicsDevice* dev) {
    sgl_Window_SwapBuffer(dev->window);
}

static void GLDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl_Framebuffer_Destroy(dev->sceneFramebuffer);

    sgl::Memory::Delete(self);
}

static void GLDevice_DrawInstanced(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount, uint32 instanceCount)
{
    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    if (va->indexCount > 0)
        glDrawElementsInstanced(GL_TRIANGLES, va->indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    else
        glDrawArraysInstanced(GL_TRIANGLES, 0, va->vertexCount, instanceCount);
}

static void GLDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount)
{
    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    if (va->indexCount > 0)
        glDrawElements(GL_TRIANGLES, va->indexCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, va->vertexCount);
}

static void GLDevice_SetVsync(sgl_GraphicsDevice* dev, bool enable) {
    SDL_GL_SetSwapInterval(enable ? 1 : 0);
}

static void GLDevice_SetScissor(sgl_GraphicsDevice* dev, bool enabled, sgl_Vec2i position, sgl_Vec2i size)
{
    if (!enabled)
    {
        glDisable(GL_SCISSOR_TEST);
        return;
    }

    glEnable(GL_SCISSOR_TEST);

    GLint flippedY = (GLint)dev->height - (position.y + size.height);
    glScissor(position.x, flippedY, size.width, size.height);
}

static void GLDevice_ImGui_Init(sgl_GraphicsDevice* dev)
{
    if (!dev->window->cfg.enableImGui)
        return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForOpenGL(dev->window->window, dev->window->glContext);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

static void GLDevice_ImGui_Shutdown(sgl_GraphicsDevice* dev)
{
    if (!dev->window->cfg.enableImGui)
            return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

static void GLDevice_ImGui_NewFrame(sgl_GraphicsDevice* dev)
{
    if (!dev->window->cfg.enableImGui)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

static void GLDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* dev)
{
    if (!dev->window->cfg.enableImGui)
            return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static const sgl_GraphicsDeviceVTable gGlVTable =
{
    .SetClearColour = &GLDevice_SetClearColour,
    .SetDepthTestEnabled = &GLDevice_SetDepthTestEnabled,
    .SetDepthWriteEnabled = &GLDevice_SetDepthWriteEnabled,
    .Resize = &GLDevice_Resize,
    .BeginFrame = &GLDevice_BeginFrame,
    .EndFrame = &GLDevice_EndFrame,
    .Destroy = &GLDevice_Destroy,
    .SwapBuffer = &GLDevice_SwapBuffer,
    .Draw = &GLDevice_Draw,
    .DrawInstanced = &GLDevice_DrawInstanced,
    .SetVsync = &GLDevice_SetVsync,
    .SetScissor = &GLDevice_SetScissor,
    .BindBackbuffer = &GLDevice_BindBackbuffer,
    .SetBlendMode = &GLDevice_SetBlendMode,

    .ImGui_Init = &GLDevice_ImGui_Init,
    .ImGui_Shutdown = &GLDevice_ImGui_Shutdown,
    .ImGui_NewFrame = &GLDevice_ImGui_NewFrame,
    .ImGui_RenderDrawData = &GLDevice_ImGui_RenderDrawData,
};

sgl_GLDevice* sgl_GLDevice_Create(sgl_Window* window)
{
    sgl_GLDevice* device = sgl::Memory::New<sgl_GLDevice>();
    device->base.clearColour = sgl_Col_CornflowerBlue;
    device->base.window = window;
    device->base.width = window->screenSize.width;
    device->base.height = window->screenSize.height;
    device->base.vtable = &gGlVTable;

    GLDevice_SetClearColour(&device->base, device->base.clearColour);
    glViewport(0, 0, device->base.width, device->base.height);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    device->base.blendMode = sgl_BlendMode_ALPHA;
    GLDevice_SetBlendMode(&device->base, sgl_BlendMode_ALPHA);

    device->base.sceneScale = 1;
    device->base.sceneFramebuffer = sgl_Framebuffer_Create((sgl_GraphicsDevice*)device, { (int)device->base.width, (int)device->base.height }, true);

    std::stringstream ss;

    ss << "SDL Version: " << SDL_VERSIONNUM_MAJOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MINOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MICRO(SDL_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "SGL Version: " << sgl_VersionString();
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "OpenGL Version: " << (const char*)glGetString(GL_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "GLSL Version: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "Vendor: " << (const char*)glGetString(GL_VENDOR);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "Renderer: " << (const char*)glGetString(GL_RENDERER);
    sgl_Log(ss.str().c_str());

    return device;
}