#pragma once
#include <sgl/Memory/Memory.h>
#include <sgl/Error.h>
#include <sgl/String.h>
#include <typeinfo>

namespace sgl
{
    template <typename T>
    class Ptr final
    {
        template <typename U>
        friend class Ptr;

    private:
        T* mPtr = nullptr;
        uint64* mRc = nullptr;

    public:
        template <typename U>
        Ptr<U> DynamicCast() const
        {
            U* castedPtr = dynamic_cast<U*>(mPtr);
            if (!castedPtr)
                return nullptr_t{};

            Ptr<U> newPtr;

            // We have to construct manually so it doesn't use a brand new ref count
            newPtr.mPtr = castedPtr;
            newPtr.mRc = mRc;

            if (mRc)
                ++(*mRc);

            return newPtr;
        }

        void Leak()
        {
            mPtr = nullptr;
            mRc = nullptr;
        }

        void Reset()
        {
            if (mRc)
            {
                --(*mRc);
                if (*mRc == 0)
                {
                    Memory::Delete(mPtr);
                    Memory::Delete(mRc);
                }

                mPtr = nullptr;
                mRc = nullptr;
            }
        }

        uint64 GetRefCount() const {
            return mRc ? *mRc : 0;
        }

        T* Get() const {
            return mPtr;
        }

        template <typename U>
        U* GetAs() const {
            return dynamic_cast<U*>(mPtr);
        }

        T& Ref() const
        {
            if (!mPtr)
                SGL_REPORT_ERROR(FormatString(u8"Attemtped to dereference a null Ptr<{}>", typeid(T).name()));

            return *mPtr;
        }

        bool operator==(nullptr_t) const {
            return mPtr == nullptr;
        }

        bool operator!=(nullptr_t) const {
            return mPtr != nullptr;
        }

        bool operator==(const Ptr& other) {
            return mPtr == other.mPtr;
        }

        bool operator!=(const Ptr& other) {
            return mPtr != other.mPtr;
        }

        explicit operator bool() const {
            return mPtr != nullptr;
        }

        T* operator->() const {
            return Get();
        }

        T& operator*() const {
            return Ref();
        }

        Ptr& operator=(nullptr_t)
        {
            Reset();
            return *this;
        }

        Ptr& operator=(T* ptr)
        {
            Reset();

            mPtr = ptr;
            mRc = Memory::New<uint64>(1);

            return *this;
        }

        Ptr& operator=(const Ptr& other)
        {
            if (this == &other)
                return *this;

            Reset();
            
            mPtr = other.mPtr;
            mRc = other.mRc;

            if (mRc)
                ++(*mRc);

            return *this;
        }

        Ptr& operator=(Ptr&& other) noexcept
        {
            if (this == &other)
                return *this;

            Reset();

            mPtr = other.mPtr;
            mRc = other.mRc;

            other.mPtr = nullptr;
            other.mRc = nullptr;

            return *this;
        }

        Ptr() :
            mPtr(nullptr),
            mRc(nullptr) {
        }

        Ptr(nullptr_t) :
            mPtr(nullptr),
            mRc(nullptr) {
        }

        Ptr(T* ptr) :
            mPtr(ptr),
            mRc(Memory::New<uint64>(1)) {
        }

        Ptr(const Ptr& other) :
            mPtr(other.mPtr),
            mRc(other.mRc)
        {
            if (mRc)
                ++(*mRc);
        }

        Ptr(Ptr&& other) noexcept :
            mPtr(other.mPtr),
            mRc(other.mRc)
        {
            other.mPtr = nullptr;
            other.mRc = nullptr;
        }

        ~Ptr() {
            Reset();
        }
    };

    template <typename T, typename... Args>
    Ptr<T> MakePtr(Args&&... args) {
        return Ptr<T>(Memory::New<T>(std::forward<Args>(args)...));
    }
}