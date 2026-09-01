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

    sgl_Model_VertexSource BuildVertexSource(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index, float scaledPosition[3])
    {
        sgl_Model_VertexSource source{};

        if (index.vertex_index >= 0)
        {
            const float* src = &attrib.vertices[3ull * index.vertex_index];
            scaledPosition[0] = src[0] * gUnitScale;
            scaledPosition[1] = src[1] * gUnitScale;
            scaledPosition[2] = src[2] * gUnitScale;
            source.position = scaledPosition;
        }

        if (index.normal_index >= 0)
            source.normal = &attrib.normals[3ull * index.normal_index];

        if (index.texcoord_index >= 0)
        {
            source.texcoord = &attrib.texcoords[2ull * index.texcoord_index];
            source.texcoordW = &attrib.texcoord_ws[index.texcoord_index];
        }

        if (!attrib.colors.empty() && index.vertex_index >= 0)
            source.colour = &attrib.colors[3ull * index.vertex_index];

        return source;
    }
}

sgl_VertexArray* sgl_Model_Load(sgl_GraphicsDevice* gpu, const char* path, uint32 vertexSize, sgl_VertexLayout* layout, sgl_Model_Vertex_Callback_ptr callback, void* userdata)
{
    try
    {
        if (!callback)
            throw std::exception("No callback provided");

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path, nullptr, true, false))
        {
            SGL_REPORT_ERROR((std::string("Failed to load model '") + path + "': " + err).c_str());
            return nullptr;
        }

        sgl_VertexArray* va = sgl_VertexArray_Create(gpu, vertexSize, layout, 0);

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

                float scaledPosition[3];
                sgl_Model_VertexSource source = BuildVertexSource(attrib, index, scaledPosition);

                callback(&source, vertex.data(), userdata);

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