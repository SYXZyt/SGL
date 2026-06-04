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
#include <SGL/Graphics/Backends/OpenGL/GLPostProcess.h>

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

static void GLDevice_Resize(sgl_GraphicsDevice* dev, sgl_Vec2i newSize)
{
    dev->width = newSize.width;
    dev->height = newSize.height;
    glViewport(0, 0, dev->width, dev->height);
}

static void GLDevice_BeginFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;
    glBindFramebuffer(GL_FRAMEBUFFER, self->sceneFBO);

    glClear(GL_COLOR_BUFFER_BIT);
}

static void GLDevice_EndFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl_PostProcess** effects;
    size_t effectCount = sgl_GraphicsDevice_GetEffects(dev, &effects);

    gluint currentTexture = self->sceneTexture;
    gluint currentFbo = self->sceneFBO;

    glDisable(GL_CULL_FACE);

    for (size_t i = 0; i < effectCount; ++i)
    {
        sgl_GLPostProcess* effect = (sgl_GLPostProcess*)effects[i];

        if (!effect->base.enabled)
            continue;

        sgl_PostProcess_Bind((sgl_PostProcess*)effect);
        sgl_VertexArray_Bind(dev->screenQuad);
        glBindTextureUnit(0, currentTexture);

        glDrawArrays(GL_TRIANGLES, 0, dev->screenQuad->vertexCount);

        currentTexture = effect->texture;
        currentFbo = effect->framebuffer;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0,
        dev->width, dev->height,
        0, 0,
        dev->width, dev->height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_CULL_FACE);

    sgl_Window_SwapBuffer(dev->window);
}

static void GLDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl::Memory::Delete(self);
}

static void GLDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount)
{
    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    glDrawArrays(GL_TRIANGLES, 0, va->vertexCount);
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
    .Resize = &GLDevice_Resize,
    .BeginFrame = &GLDevice_BeginFrame,
    .EndFrame = &GLDevice_EndFrame,
    .Destroy = &GLDevice_Destroy,
    .Draw = &GLDevice_Draw,

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCreateTextures(GL_TEXTURE_2D, 1, &device->sceneTexture);
    glTextureStorage2D(device->sceneTexture, 1, GL_RGBA8, (GLsizei)window->screenSize.width, (GLsizei)window->screenSize.height);

    glCreateFramebuffers(1, &device->sceneFBO);
    glNamedFramebufferTexture(device->sceneFBO, GL_COLOR_ATTACHMENT0, device->sceneTexture, 0);

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