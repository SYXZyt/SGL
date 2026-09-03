#include "GraphicsDevice.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Backends/OpenGL/GLDevice.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <vector>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/PostProcess.h>

static std::vector<sgl_PostProcess*>& GetEffectList(void* ptr) {
    return *((std::vector<sgl_PostProcess*>*)ptr);
}

struct alignas(16) PP_Vertex
{
    sgl_Vec3 pos;
    sgl_Vec2 uv;
};

static void OnResize(sgl_Window* window, sgl_Vec2i newSize, void* userdata)
{
    sgl_GraphicsDevice* device = (sgl_GraphicsDevice*)userdata;

    auto& effects = GetEffectList(device->vecPtr);
    for (sgl_PostProcess* effect : effects)
        sgl_PostProcess_OnResize(effect);
        
    device->vtable->Resize(device, newSize);
}

sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window)
{
    sgl_GraphicsDevice* device = nullptr;

    sgl_VertexLayout* screenQuadLayout = sgl_VertexLayout_New(device);
    {
        sgl_VertexElement pos
        {
            .semantic = sgl_POSITION,
            .offset = offsetof(PP_Vertex, pos),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexElement uv
        {
            .semantic = sgl_TEXCOORD,
            .offset = offsetof(PP_Vertex, uv),
            .type = sgl_VertexElementType_VEC2,
            .perInstance = false,
        };

        sgl_VertexLayout_Add(screenQuadLayout, pos);
        sgl_VertexLayout_Add(screenQuadLayout, uv);
    }

    if (window->cfg.backend == sgl_Backend_OPENGL)
    {
        device = (sgl_GraphicsDevice*)sgl_GLDevice_Create(window);
    }
    else if (window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        device = (sgl_GraphicsDevice*)sgl_DXDevice_Create(window, screenQuadLayout);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    device->depthTestEnabled = true;
    device->depthWriteEnabled = true;

    device->screenQuadLayout = screenQuadLayout;
    device->screenQuad = sgl_VertexArray_Create(device, sizeof(PP_Vertex), device->screenQuadLayout, 0);

    PP_Vertex tl = { .pos = {{{-1,  1, 0}}}, .uv = {{{0, 1}}} };
    PP_Vertex tr = { .pos = {{{ 1,  1, 0}}}, .uv = {{{1, 1}}} };
    PP_Vertex bl = { .pos = {{{-1, -1, 0}}}, .uv = {{{0, 0}}} };
    PP_Vertex br = { .pos = {{{ 1, -1, 0}}}, .uv = {{{1, 0}}} };

    // DirectX wants the Y uv flipping, otherwise each effect will cause the next buffer to flip
    if (window->cfg.backend == sgl_Backend_DIRECTX11)
    {
        tl.uv.y *= -1;
        tr.uv.y *= -1;
        bl.uv.y *= -1;
        br.uv.y *= -1;
    }

    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(device->screenQuad, q);

    sgl_Window_RegisterResize(window, &OnResize, device);

    device->vecPtr = sgl::Memory::New<std::vector<sgl_PostProcess*>>();
    return device;
}

void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device)
{
    std::vector<sgl_PostProcess*>* effectList = (std::vector<sgl_PostProcess*>*)device->vecPtr;

    for (sgl_PostProcess* effect : *effectList)
        sgl_PostProcess_Destroy(effect);

    sgl::Memory::Delete(effectList);

    sgl_VertexLayout_Destroy(device->screenQuadLayout);
    sgl_VertexArray_Destroy(device->screenQuad); // base creates it so base should also destroy it
    sgl_Window_DeregisterResize(device->window, &OnResize);
    device->vtable->Destroy(device);
}

void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour) {
    device->vtable->SetClearColour(device, colour);
}

void sgl_GraphicsDevice_SetDepthTestEnabled(sgl_GraphicsDevice* device, bool enabled)
{
    device->depthTestEnabled = enabled;
    device->vtable->SetDepthTestEnabled(device, enabled);
}

bool sgl_GraphicsDevice_GetDepthTestEnabled(sgl_GraphicsDevice* device) {
    return device->depthTestEnabled;
}

void sgl_GraphicsDevice_SetDepthWriteEnabled(sgl_GraphicsDevice* device, bool enabled)
{
    device->depthWriteEnabled = enabled;
    device->vtable->SetDepthWriteEnabled(device, enabled);
}

bool sgl_GraphicsDevice_GetDepthWriteEnabled(sgl_GraphicsDevice* device) {
    return device->depthWriteEnabled;
}

void sgl_GraphicsDevice_BeginFrame(sgl_GraphicsDevice* device) {
    device->vtable->BeginFrame(device);
}

void sgl_GraphicsDevice_EndFrame(sgl_GraphicsDevice* device) {
    device->vtable->EndFrame(device);
}

void sgl_GraphicsDevice_SwapBuffer(sgl_GraphicsDevice* device) {
    device->vtable->SwapBuffer(device);
}

void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount) {
    device->vtable->Draw(device, va, shader, textures, textureCount, buffers, bufferCount);
}

void sgl_GraphicsDevice_DrawInstanced(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount, uint32 instanceCount) {
    device->vtable->DrawInstanced(device, va, shader, textures, textureCount, buffers, bufferCount, instanceCount);
}

void sgl_GraphicsDevice_AddEffect(sgl_GraphicsDevice* device, sgl_PostProcess* effect)
{
    auto& effects = GetEffectList(device->vecPtr);
    effects.push_back(effect);
}

size_t sgl_GraphicsDevice_GetEffects(sgl_GraphicsDevice* device, sgl_PostProcess*** outEffects)
{
    auto& effects = GetEffectList(device->vecPtr);
    *outEffects = effects.data();

    return effects.size();
}

sgl_VertexLayout* sgl_GraphicsDevice_GetPostProcessLayout(sgl_GraphicsDevice* device) {
    return device->screenQuadLayout;
}

void sgl_GraphicsDevice_ImGui_Init(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_Init(device);
}

void sgl_GraphicsDevice_ImGui_Shutdown(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_Shutdown(device);
}

void sgl_GraphicsDevice_ImGui_NewFrame(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_NewFrame(device);
}

void sgl_GraphicsDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_RenderDrawData(device);
}