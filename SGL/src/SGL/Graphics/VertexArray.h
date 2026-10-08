#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

struct sgl_VertexArray;
typedef struct sgl_VertexArrayVTable sgl_sealed
{
    void (*Destroy)(struct sgl_VertexArray* self);
    void (*Bind)(struct sgl_VertexArray* self);
} sgl_VertexArrayVTable;

/// @brief A GPU model. Optionially has indices
typedef struct sgl_VertexArray sgl_sealed
{
    const sgl_VertexArrayVTable* vtable;
    
    sgl_GraphicsDevice* gpu;

    sgl_VertexLayout* layout;

    byte* vertexData;
    uint32 vertexCount;
    uint32 vertexCapacity;
    uint32 vertexSize;

    // If indexCount is 0, backends draw with the non-indexed path
    uint32* indexData;
    uint32 indexCount;
    uint32 indexCapacity;

    // If instanceSize is 0, this VertexArray was not created with instancing enabled
    byte* instanceData;
    uint32 instanceCount;
    uint32 instanceCapacity;
    uint32 instanceSize;

    bool needsVertexUpload;
    bool needsIndexUpload;
    bool needsInstanceUpload;
    bool layoutDirty;
} sgl_VertexArray;

typedef struct sgl_VertexArray_Quad sgl_sealed
{
    void* tl;
    void* tr;
    void* bl;
    void* br;
} sgl_VertexArray_Quad;

typedef struct sgl_VertexArray_Tri sgl_sealed
{
    void* v0;
    void* v1;
    void* v2;
} sgl_VertexArray_Tri;

typedef struct sgl_VertexArray_Triangulated sgl_sealed
{
    sgl_VertexArray_Tri t0;
    sgl_VertexArray_Tri t1;
} sgl_VertexArray_Triangulated;

/// @brief Triangulate a quad into 2 tris
/// @param tl Pointer to the top left vertex
/// @param tr Pointer to the top right vertex
/// @param br Pointer to the bottom right vertex
/// @param bl Pointer to the bottom left vertex
SGL_API extern sgl_VertexArray_Triangulated sgl_Triangulate(void* tl, void* tr, void* br, void* bl);

/// @brief Create an empty vertex array
/// @param gpu The device to use
/// @param vertexSize How large a vertex is, in bytes.
/// @param layout The layout of the vertex
/// @param instanceSize How large a single instance's data is, in bytes. Pass 0 to disable instancing for this buffer
/// @return
SGL_API extern sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize, sgl_VertexLayout* layout, uint32 instanceSize);

/// @brief Bind a vertex array
/// @param va The buffer to bind
SGL_API extern void sgl_VertexArray_Bind(sgl_VertexArray* va);

/// @brief Destroy a vertex array
/// @param va The buffer to destroy
SGL_API extern void sgl_VertexArray_Destroy(sgl_VertexArray* va);

/// @brief Set a vertex array to an existing vertex array
/// @param va The buffer to use
/// @param vertices Array of vertices
/// @param vertexCount How many vertices to load
SGL_API extern void sgl_VertexArray_Set(sgl_VertexArray* va, byte* vertices, uint32 vertexCount);

/// @brief Add a vertex to the model
/// @param va The buffer to use
/// @param vertex Pointer to a vertex
SGL_API extern void sgl_VertexArray_AddVertex(sgl_VertexArray* va, void* vertex);

/// @brief Add a quad to the model
/// @param va The buffer to use
/// @param quad The quad to add
SGL_API extern void sgl_VertexArray_AddQuad(sgl_VertexArray* va, sgl_VertexArray_Quad quad);

/// @brief Add a tri to the model
/// @param va The buffer to use
/// @param tri The tri to add
SGL_API extern void sgl_VertexArray_AddTri(sgl_VertexArray* va, sgl_VertexArray_Tri tri);

/// @brief Set the indices of a buffer
/// @param va The buffer to use
/// @param indices Array of indices to use
/// @param indexCount How many indices
SGL_API extern void sgl_VertexArray_SetIndices(sgl_VertexArray* va, const uint32* indices, uint32 indexCount);

/// @brief Add an index to the model
/// @param va The buffer to use
/// @param index The index to add
SGL_API extern void sgl_VertexArray_AddIndex(sgl_VertexArray* va, uint32 index);

/// @brief Set the per-instance data of a buffer to an existing array. Requires the buffer to have been created with instanceSize > 0
/// @param va The buffer to use
/// @param instances Array of instance data
/// @param instanceCount How many instances to load
SGL_API extern void sgl_VertexArray_SetInstances(sgl_VertexArray* va, byte* instances, uint32 instanceCount);

/// @brief Add a single instance's data to the buffer. Requires the buffer to have been created with instanceSize > 0
/// @param va The buffer to use
/// @param instance Pointer to the instance data
SGL_API extern void sgl_VertexArray_AddInstance(sgl_VertexArray* va, void* instance);

/// @brief Add the indices for a tri
/// @param va The bufer to use
/// @param i0 Index 0
/// @param i1 Index 1
/// @param i2 Index 2
SGL_API extern void sgl_VertexArray_AddTriIndices(sgl_VertexArray* va, uint32 i0, uint32 i1, uint32 i2);

SGL_END