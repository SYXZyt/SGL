#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Util/String.h>

SGL_BEGIN

/// @brief What data type is this element
typedef uint8 sgl_VertexElementType;
enum
{
    sgl_VertexElementType_FLOAT,

    sgl_VertexElementType_INT8,
    sgl_VertexElementType_INT16,
    sgl_VertexElementType_INT32,

    sgl_VertexElementType_UINT8,
    sgl_VertexElementType_UINT16,
    sgl_VertexElementType_UINT32,

    sgl_VertexElementType_VEC2,
    sgl_VertexElementType_VEC3,
    sgl_VertexElementType_VEC4,
};

/// @brief What this element is storing
typedef uint8 sgl_VertexElementSemantic;
enum
{
    sgl_POSITION,
    sgl_COLOUR,
    sgl_TEXCOORD,
    sgl_NORMAL,
};

/// @brief Description of one element for a vertex
typedef struct sgl_VertexElement sgl_sealed
{
    sgl_VertexElementSemantic semantic;
    size_t offset;
    sgl_VertexElementType type;
    bool perInstance; // Unused for now    
} sgl_VertexElement;

/// @brief List of element descriptions for a vertex
typedef struct sgl_VertexLayout sgl_sealed
{
    sgl_GraphicsDevice* gpu;

    sgl_VertexElement* elements;
    uint32 elementCount;
    uint32 elementCapacity;
} sgl_VertexLayout;

/// @brief Create a new vertex layout
/// @param gpu The device to use
SGL_API extern sgl_VertexLayout* sgl_VertexLayout_New(sgl_GraphicsDevice* gpu);

/// @brief Create a copy of an existing layout
/// @param layout The layout to copy
/// @return The copy
SGL_API extern sgl_VertexLayout* sgl_VertexLayout_DeepCopy(sgl_VertexLayout* layout);

/// @brief Add a new element to the layout
/// @param layout The layout to use
/// @param element The element to add
SGL_API extern void sgl_VertexLayout_Add(sgl_VertexLayout* layout, sgl_VertexElement element);

/// @brief Destroy a vertex layout
/// @param layout The layout to destroy
SGL_API extern void sgl_VertexLayout_Destroy(sgl_VertexLayout* layout);

SGL_END