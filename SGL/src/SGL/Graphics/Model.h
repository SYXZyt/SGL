#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>

SGL_BEGIN

SGL_API extern float sgl_Model_GetUnitScale();
SGL_API extern void sgl_Model_SetUnitScale(float unitScale);

SGL_API extern sgl_VertexArray* sgl_Model_Load(sgl_GraphicsDevice* gpu, const char* path, uint32 vertexSize, sgl_VertexLayout* layout);

SGL_END
