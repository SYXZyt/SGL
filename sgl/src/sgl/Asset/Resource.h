#pragma once
#include <sgl/sgl.h>
#include <sgl/Ptr.h>
#include <sgl/Graphics/Shader.h>
#include <thread>
#include <sgl/FileSystem.h>

namespace sgl
{
    template <typename T>
    class Resource final
    {
    public:
        static Ptr<T> LoadFromFile(const String& path)
        {
            if constexpr (std::same_as<T, Graphics::Shader>)
            {
                Ptr ptr = MakePtr<Graphics::Shader>();
                
                std::thread([ptr, path]()
            {
                String vcode = FileSystem::ReadFile(path + u8"_vs.glsl");
                String fcode = FileSystem::ReadFile(path + u8"_fs.glsl");

                ptr->CreateShader(vcode, fcode);
            }).detach();

                return ptr;
            }
            else
            {
                SGL_REPORT_ERROR(u8"Could not load resource");
                return nullptr;
            }
        }

        Resource() = delete;
    };
}