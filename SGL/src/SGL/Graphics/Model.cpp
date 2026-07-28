#include "Model.h"
#include <SGL/Util/Error.h>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <string>
#include <format>

static float gUnitScale = 100.f;

namespace
{
    struct IndexKey
    {
        int position, normal, texcoord;

        bool operator==(const IndexKey& other) const {
            return position == other.position && normal == other.normal && texcoord == other.texcoord;
        }
    };

    struct IndexKeyHash
    {
        size_t operator()(const IndexKey& key) const
        {
            size_t h = std::hash<int>()(key.position);
            h = h * 31 + std::hash<int>()(key.normal);
            h = h * 31 + std::hash<int>()(key.texcoord);
            return h;
        }
    };

    void WriteFloats(byte* vertex, size_t offset, const float* src, int count) {
        std::memcpy(vertex + offset, src, sizeof(float) * count);
    }
}

sgl_VertexArray* sgl_Model_Load(sgl_GraphicsDevice* gpu, const char* path, uint32 vertexSize, sgl_VertexLayout* layout)
{
    try
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path))
        {
            SGL_REPORT_ERROR((std::string("Failed to load model '") + path + "': " + err).c_str());
            return nullptr;
        }

        sgl_VertexArray* va = sgl_VertexArray_Create(gpu, vertexSize, layout);

        std::vector<byte> vertex(vertexSize);
        std::unordered_map<IndexKey, uint32, IndexKeyHash> uniqueVertices;

        for (const tinyobj::shape_t& shape : shapes)
        {
            for (const tinyobj::index_t& index : shape.mesh.indices)
            {
                IndexKey key{ index.vertex_index, index.normal_index, index.texcoord_index };

                auto existing = uniqueVertices.find(key);
                if (existing != uniqueVertices.end())
                {
                    sgl_VertexArray_AddIndex(va, existing->second);
                    continue;
                }

                std::memset(vertex.data(), 0, vertexSize);

                for (uint32 i = 0; i < layout->elementCount; ++i)
                {
                    const sgl_VertexElement& element = layout->elements[i];

                    switch (element.semantic)
                    {
                        case sgl_POSITION:
                            if (index.vertex_index >= 0)
                            {
                                const float* src = &attrib.vertices[3ull * index.vertex_index];
                                float scaled[3] = { src[0] * gUnitScale, src[1] * gUnitScale, src[2] * gUnitScale };
                                WriteFloats(vertex.data(), element.offset, scaled, 3);
                            }
                            break;

                        case sgl_NORMAL:
                            if (index.normal_index >= 0)
                                WriteFloats(vertex.data(), element.offset, &attrib.normals[3ull * index.normal_index], 3);
                            break;

                        case sgl_TEXCOORD:
                            if (index.texcoord_index >= 0)
                                WriteFloats(vertex.data(), element.offset, &attrib.texcoords[2ull * index.texcoord_index], 2);
                            break;

                        default:
                            break;
                    }
                }

                sgl_VertexArray_AddVertex(va, vertex.data());

                uint32 newIndex = va->vertexCount - 1;
                uniqueVertices.emplace(key, newIndex);
                sgl_VertexArray_AddIndex(va, newIndex);
            }
        }

        return va;
    }
    catch (const std::exception& e)
    {
        SGL_REPORT_ERROR(std::format("Failed to load model '{}' : {}", path, e.what()).c_str());
        return nullptr;
    }
}

float sgl_Model_GetUnitScale() {
    return gUnitScale;
}

void sgl_Model_SetUnitScale(float unitScale) {
    gUnitScale = unitScale;
}