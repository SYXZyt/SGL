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

/// @brief Load a model into a vertex array
/// @param gpu The device to use
/// @param path The path of the file on disk
/// @param vertexSize The size in bytes of the vertex 
/// @param layout The vertex layout to use
/// @return An indexed vertex array
SGL_API extern sgl_VertexArray* sgl_Model_Load(sgl_GraphicsDevice* gpu, const char* path, uint32 vertexSize, sgl_VertexLayout* layout);

SGL_END
