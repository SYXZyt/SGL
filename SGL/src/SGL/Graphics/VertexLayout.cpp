#include "VertexLayout.h"
#include <SGL/Util/Error.h>
#include <SGL/Util/Memory.h>

static void EnsureCapacity(sgl_VertexLayout* layout)
{
    if (layout->elementCount < layout->elementCapacity)
        return;

    uint32 newCap = layout->elementCapacity == 0 ? 4 : layout->elementCapacity * 2;

    layout->elements = (sgl_VertexElement*)sgl_Realloc(layout->elements, newCap * sizeof(sgl_VertexElement));
    layout->elementCapacity = newCap;
}

sgl_VertexLayout* sgl_VertexLayout_New(sgl_GraphicsDevice* gpu)
{
    sgl_VertexLayout* layout = sgl::Memory::New<sgl_VertexLayout>();
    
    layout->elements = nullptr;
    layout->elementCount = 0;
    layout->elementCapacity = 0;
    layout->gpu = gpu;

    return layout;
}

sgl_VertexLayout* sgl_VertexLayout_DeepCopy(sgl_VertexLayout* layout)
{
    sgl_VertexLayout* newLayout = sgl::Memory::New<sgl_VertexLayout>();

    newLayout->elementCapacity = layout->elementCapacity;
    newLayout->elementCount = layout->elementCount;
    newLayout->elements = (sgl_VertexElement*)sgl_Malloc(newLayout->elementCapacity * sizeof(sgl_VertexElement));

    for (uint32 i = 0; i < newLayout->elementCount; ++i)
    {
        sgl_VertexElement& src = layout->elements[i];
        sgl_VertexElement& dst = newLayout->elements[i];

        dst.semantic = sgl_MakeString(src.semantic.str);
        dst.type = src.type;
        dst.offset = src.offset;
        dst.perInstance = src.perInstance;
    }

    return newLayout;
}

void sgl_VertexLayout_Add(sgl_VertexLayout* layout, sgl_VertexElement element)
{
    EnsureCapacity(layout);
    layout->elements[layout->elementCount++] = element;
}

void sgl_VertexLayout_Destroy(sgl_VertexLayout* layout)
{
    for (uint32 i = 0; i < layout->elementCount; ++i)
        sgl_FreeString(layout->elements[i].semantic);

    sgl_Free(layout->elements);
    sgl::Memory::Delete(layout);
}