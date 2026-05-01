#include "GraphicsDevice.h"
#include <sgl/Graphics/OpenGL/GLDevice.h>

sgl::ClearFlag sgl::operator|(ClearFlag a, ClearFlag b) {
    return (ClearFlag)((uint8)a | (uint8)b);
}

sgl::ClearFlag sgl::operator&(ClearFlag a, ClearFlag b) {
    return (ClearFlag)((uint8)a & (uint8)b);
}

sgl::ClearFlag& sgl::operator|=(ClearFlag& a, ClearFlag b)
{
    a = (ClearFlag)((uint8)a | (uint8)b);
    return a;
}

sgl::ClearFlag& sgl::operator&=(ClearFlag& a, ClearFlag b)
{
    a = (ClearFlag)((uint8)a & (uint8)b);
    return a;
}

sgl::Ptr<sgl::GraphicsDevice> sgl::GraphicsDevice::Create(GraphicsAPI api)
{
    switch (api)
    {
        case GraphicsAPI::OPENGL:
            return sgl::MakePtr<Graphics::GLDevice>().DynamicCast<GraphicsDevice>();
        default:
            SGL_REPORT_ERROR(u8"Graphics api is not supported");
            break;
    }

    return nullptr;
}