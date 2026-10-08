#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>

SGL_BEGIN

/// @brief Get the unit scale
SGL_API extern float sgl_Model_GetUnitScale();

/// @brief Set the unit scale. This will be used to scale the model
/// @param unitScale The scale to use
SGL_API extern void sgl_Model_SetUnitScale(float unitScale);

/// @brief Contains data from a loaded model. Use this to load into your own vertex struct
typedef struct sgl_Model_VertexSource sgl_sealed
{
    const float* position;  // xyz or NULL
    const float* normal;    // xyz or NULL
    const float* texcoord;  // uv or NULL
    const float* texcoordW; // contains the w component of the texcoord
    const float* colour;    // vertex colour or NULL
} sgl_Model_VertexSource;

/// @brief Convert a vertex to a custom one. Will be called per-vertex
typedef sglFuncPtr(sgl_Model_Vertex_Callback_ptr, void, const sgl_Model_VertexSource* source, void* outVertex, void* userdata);

/// @brief Load a model into a vertex array
/// @param gpu The device to use
/// @param path The path of the file on disk
/// @param vertexSize The size in bytes of the vertex
/// @param layout The vertex layout to use
/// @param callback Called once per unique vertex to populate it from the parsed source data. Cannot be NULL
/// @param userdata Any extra data the callback may need
/// @return An indexed vertex array
SGL_API extern sgl_VertexArray* sgl_Model_Load(sgl_GraphicsDevice* gpu, const char* path, uint32 vertexSize, sgl_VertexLayout* layout, sgl_Model_Vertex_Callback_ptr callback, void* userdata);

SGL_END
