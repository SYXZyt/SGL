#pragma once
#include <sgl/sgl.h>
#include <sgl/Collections/List.h>
#include <glad/glad.h>
#include <cstring>
#include <sgl/Graphics/VertexLayout.h>

namespace sgl
{
    class VertexArray final
    {
        template <typename T>
        struct Iterator
        {
            using value_type = T;
            using reference = T&;
            using pointer = T*;
            using difference_type = ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            Iterator(byte* ptr, uint64 stride)
                : mPtr(ptr), mStride(stride)
            {
            }

            reference operator*() const
            {
                return *(T*)mPtr;
            }

            pointer operator->() const
            {
                return (T*)mPtr;
            }

            Iterator& operator++()
            {
                mPtr += mStride;
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator tmp = *this;
                mPtr += mStride;
                return tmp;
            }

            bool operator==(const Iterator& other) const
            {
                return mPtr == other.mPtr;
            }

            bool operator!=(const Iterator& other) const
            {
                return mPtr != other.mPtr;
            }

        private:
            byte* mPtr = nullptr;
            uint64 mStride = 0;
        };

    public:
        template <typename T>
        struct Quad final
        {
            T tl;
            T tr;
            T bl;
            T br;
        };

        template <typename T>
        struct Tri final
        {
            T v0;
            T v1;
            T v2;
        };

        template <typename T>
        struct Triangulated final
        {
            T t0_0;
            T t0_1;
            T t0_2;

            T t1_0;
            T t1_1;
            T t1_2;
        };

    private:
        std::vector<uint32> mIndexData;
        std::vector<byte> mVertexData;

        VertexLayout mLayout;

        size_t mVertexSize = 0;
        size_t mT = 0;

        gluint mVAO = 0;
        gluint mVBO = 0;
        gluint mEBO = 0;

        mutable bool mNeedsUpdate = true;
        mutable bool mIndexNeedsUpdate = true;
        bool mLayoutDirty = true;

        bool mIsIndexed = true;

        template <typename T>
        Triangulated<T> Triangulate(const Quad<T>& quad) const {
            return Triangulate(quad.tl, quad.tr, quad.br, quad.bl);
        }

        template <typename T>
        Triangulated<T> Triangulate(const T& tl, const T& tr, const T& br, const T& bl) const
        {
            Triangulated<T> result{};

            result.t0_0 = tl;
            result.t0_1 = tr;
            result.t0_2 = br;

            result.t1_0 = tl;
            result.t1_1 = br;
            result.t1_2 = bl;

            return result;
        }

        void AddRawVertex(const void* vertex)
        {
            const byte* bytes = (const byte*)vertex;
            mVertexData.insert(mVertexData.end(), bytes, bytes + mVertexSize);
            mNeedsUpdate = true;
        }

        uint32 AddRawVertexAndGetIndex(const void* vertex)
        {
            uint32 index = (uint32)VertexCount();
            AddRawVertex(vertex);
            return index;
        }

        void AddIndex(uint32 index)
        {
            mIndexData.push_back(index);
            mIndexNeedsUpdate = true;
        }

        void SetupLayout() const;

    public:
        void SetLayout(const VertexLayout& layout)
        {
            mLayout = layout;
            mLayoutDirty = true;
        }

        template <typename T>
        static VertexArray Create(bool isIndexed = true) {
            return VertexArray(sizeof(T), typeid(T).hash_code(), isIndexed);
        }

        template <typename T>
        Iterator<T> begin() {
            return Iterator<T>(mVertexData.data(), mVertexSize);
        }

        template <typename T>
        Iterator<T> end() {
            return Iterator<T>(mVertexData.data() + mVertexData.size(), mVertexSize);
        }

        template <typename T>
        void Set(sgl::List<T>& vertices, sgl::List<uint32>& indices)
        {
            mVertexData.resize(vertices.size() * sizeof(T));
            mIndexData.resize(indices.size());

            std::memcpy(mVertexData.data(), vertices.data(), vertices.size() * sizeof(T));
            std::memcpy(mIndexData.data(), indices.data(), indices.size() * sizeof(uint32));

            mNeedsUpdate = true;
            mIndexNeedsUpdate = true;
        }

        size_t VertexCount() const {
            return mVertexData.size() / mVertexSize;
        }

        size_t IndexCount() const {
            return mIndexData.size();
        }

        bool IsIndexed() const {
            return mIsIndexed;
        }

        template <typename T>
        void Add(const T& vertex)
        {
            if (mT != typeid(T).hash_code())
                return;

            if (mIsIndexed)
            {
                uint32 index = AddRawVertexAndGetIndex(&vertex);
                AddIndex(index);
            }
            else
                AddRawVertex(&vertex);
        }

        template <typename T>
        void Add(const Quad<T>& quad)
        {
            if (!mIsIndexed)
            {
                Triangulated<T> tri = Triangulate(quad);

                AddRawVertex(&tri.t0_0);
                AddRawVertex(&tri.t0_1);
                AddRawVertex(&tri.t0_2);

                AddRawVertex(&tri.t1_0);
                AddRawVertex(&tri.t1_1);
                AddRawVertex(&tri.t1_2);

                return;
            }

            uint32 i0 = AddRawVertexAndGetIndex(&quad.tl);
            uint32 i1 = AddRawVertexAndGetIndex(&quad.tr);
            uint32 i2 = AddRawVertexAndGetIndex(&quad.br);
            uint32 i3 = AddRawVertexAndGetIndex(&quad.bl);

            AddIndex(i0);
            AddIndex(i1);
            AddIndex(i2);

            AddIndex(i0);
            AddIndex(i2);
            AddIndex(i3);
        } 

        template <typename T>
        void Add(const Tri<T>& tri)
        {
            if (!mIsIndexed)
            {
                AddRawVertex(&tri.v0);
                AddRawVertex(&tri.v1);
                AddRawVertex(&tri.v2);

                return;
            }

            uint32 i0 = AddRawVertexAndGetIndex(&tri.v0);
            uint32 i1 = AddRawVertexAndGetIndex(&tri.v1);
            uint32 i2 = AddRawVertexAndGetIndex(&tri.v2);

            AddIndex(i0);
            AddIndex(i1);
            AddIndex(i2);
        }

        SGL_API void Bind();

        VertexArray(size_t size, size_t typeId, bool isIndexed = true) :
            mVertexSize(size),
            mT(typeId),
            mIsIndexed(isIndexed) {
        }
    };
}