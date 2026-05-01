#pragma once
#include <sgl/sgl.h>

namespace sgl
{
    class IResource
    {
    protected:
        enum class InitState : uint8
        {
            /// @brief Data for this object is loading. Any attempts to use this should be yielded
            LOADING,

            /// @brief Object data is loaded and just needs main thread initialising
            LOADED,

            /// @brief Object is fully loaded and ready for use
            INITIALISED,
        } mInitState = InitState::LOADING;

    public:
        virtual void Initialise() = 0;
        bool IsInitialised() const {
            return mInitState == InitState::INITIALISED;
        }

        bool IsLoading() const {
            return mInitState == InitState::LOADING;
        }

        bool CanInit() const {
            return mInitState == InitState::LOADED;
        }

        virtual ~IResource() = default;
    };
}