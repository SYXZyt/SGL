#pragma once
#include <sgl/sgl.h>
#include <sgl/Asset/IResource.h>
#include <sgl/String.h>

namespace sgl
{
    template <typename T>
    class Resource;
}

namespace sgl::Graphics
{
    class Shader final : public IResource
    {
        friend class Resource<Shader>;

    private:
        struct
        {
            String vcode, fcode;
        } mShaderData;

        gluint mShaderId;

        void CreateShader(const String& vcode, const String& fcode);

    public:
        void Initialise() override;

        void Bind();

        ~Shader();
    };
}