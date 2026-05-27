#include "VertexLayout.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLVertexLayout.h>

sgl_VertexLayout* sgl_VertexLayout_New(sgl_GraphicsDevice* gpu)
{
    sgl_VertexLayout* layout = nullptr;

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
        layout = (sgl_VertexLayout*)sgl_GLVertexLayout_Create();
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    layout->gpu = gpu;
    return layout;
}

void sgl_VertexLayout_Add(sgl_VertexLayout* layout, sgl_VertexElementType type) {
    layout->vtable->Add(layout, type);
}

void sgl_VertexLayout_Destroy(sgl_VertexLayout* layout) {
    layout->vtable->Destroy(layout);
}

void sgl_VertexLayout_Bind(sgl_VertexLayout* layout) {
    layout->vtable->Bind(layout);
}

uint32 sgl_VertexLayout_GetTypeSize(sgl_VertexElementType type)
{
    switch (type)
    {
        case sgl_VertexElementType_FLOAT:
        case sgl_VertexElementType_FLOAT2:
        case sgl_VertexElementType_FLOAT3:
        case sgl_VertexElementType_FLOAT4:
        case sgl_VertexElementType_UINT:
        case sgl_VertexElementType_UINT2:
        case sgl_VertexElementType_UINT3:
        case sgl_VertexElementType_UINT4:
            return 4;
    }

    return 0;
}

uint32 sgl_VertexLayout_GetComponentCount(sgl_VertexElementType type)
{
    switch (type)
    {
        case sgl_VertexElementType_FLOAT:
        case sgl_VertexElementType_UINT:
            return 1;

        case sgl_VertexElementType_FLOAT2:
        case sgl_VertexElementType_UINT2:
            return 2;

        case sgl_VertexElementType_FLOAT3:
        case sgl_VertexElementType_UINT3:
            return 3;

        case sgl_VertexElementType_FLOAT4:
        case sgl_VertexElementType_UINT4:
            return 4;
    }

    return 0;
}