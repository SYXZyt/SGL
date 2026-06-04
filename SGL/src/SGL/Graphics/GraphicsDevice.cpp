#include "GraphicsDevice.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Backends/OpenGL/GLDevice.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <vector>
#include <SGL/Util/Memory.h>

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
    device->vtable->Resize(device, newSize);
}

sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window)
{
    sgl_GraphicsDevice* device = nullptr;

    if (window->cfg.backend == sgl_Backend_OPENGL)
    {
        device = (sgl_GraphicsDevice*)sgl_GLDevice_Create(window);
    }
    else if (window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        device = (sgl_GraphicsDevice*)sgl_DXDevice_Create(window);
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

    device->screenQuadLayout = sgl_VertexLayout_New(device);

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

        sgl_VertexLayout_Add(device->screenQuadLayout, pos);
        sgl_VertexLayout_Add(device->screenQuadLayout, uv);
    }

    device->screenQuad = sgl_VertexArray_Create(device, sizeof(PP_Vertex), device->screenQuadLayout);

    PP_Vertex tl = { .pos = {{{-1,  1, 0}}}, .uv = {{{0, 1}}} };
    PP_Vertex tr = { .pos = {{{ 1,  1, 0}}}, .uv = {{{1, 1}}} };
    PP_Vertex bl = { .pos = {{{-1, -1, 0}}}, .uv = {{{0, 0}}} };
    PP_Vertex br = { .pos = {{{ 1, -1, 0}}}, .uv = {{{1, 0}}} };

    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(device->screenQuad, q);

    sgl_Window_RegisterResize(window, &OnResize, device);

    device->vecPtr = sgl::Memory::New<std::vector<sgl_PostProcess*>>();
    return device;
}

void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device)
{
    std::vector<sgl_PostProcess*>* effectList = (std::vector<sgl_PostProcess*>*)device->vecPtr;
    sgl::Memory::Delete(effectList);

    sgl_VertexLayout_Destroy(device->screenQuadLayout);
    sgl_VertexArray_Destroy(device->screenQuad); // base creates it so base should also destroy it
    sgl_Window_DeregisterResize(device->window, &OnResize);
    device->vtable->Destroy(device);
}

void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour) {
    device->vtable->SetClearColour(device, colour);
}

void sgl_GraphicsDevice_BeginFrame(sgl_GraphicsDevice* device) {
    device->vtable->BeginFrame(device);
}

void sgl_GraphicsDevice_EndFrame(sgl_GraphicsDevice* device) {
    device->vtable->EndFrame(device);
}

void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount) {
    device->vtable->Draw(device, va, shader, textures, textureCount, buffers, bufferCount);
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