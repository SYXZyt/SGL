#ifdef SGL_DIRECTX
#include "DXShader.h"
#include <SGL/Util/Error.h>
#include <sstream>
#include <d3dcompiler.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <vector>
#include <SGL/Util/Memory.h>

#define GetSelf sgl_DXShader* self = (sgl_DXShader*)shr

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

static DXGI_FORMAT ToDXGIFormat(sgl_VertexElementType type)
{
    switch (type)
    {
        case sgl_VertexElementType_FLOAT:
            return DXGI_FORMAT_R32_FLOAT;

        case sgl_VertexElementType_VEC2:
            return DXGI_FORMAT_R32G32_FLOAT;

        case sgl_VertexElementType_VEC3:
            return DXGI_FORMAT_R32G32B32_FLOAT;

        case sgl_VertexElementType_VEC4:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        case sgl_VertexElementType_UINT8:
            return DXGI_FORMAT_R8_UINT;

        case sgl_VertexElementType_UINT16:
            return DXGI_FORMAT_R16_UINT;

        case sgl_VertexElementType_UINT32:
            return DXGI_FORMAT_R32_UINT;

        default:
            return DXGI_FORMAT_UNKNOWN;
    }
}

static const char* ToSemanticName(sgl_VertexElementSemantic semantic)
{
    switch (semantic)
    {
        case sgl_POSITION:
            return "POSITION";

        case sgl_COLOUR:
            return "COLOR";

        case sgl_TEXCOORD:
            return "TEXCOORD";

        case sgl_NORMAL:
            return "NORMAL";
    }

    return "";
}

static void DXInitialise(sgl_Shader* shr)
{
    GetSelf;

    ID3DBlob* errors = nullptr;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG;
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompile(
        shr->data_vcode.str,
        shr->data_vcode.len,
        nullptr,
        nullptr,
        nullptr,
        shr->vertexEntryName,
        "vs_5_0",
        flags,
        0,
        &self->vertexBlob,
        &errors
    );

    if (FAILED(hr))
    {
        ReportHRError((const char*)errors->GetBufferPointer(), hr);
        return;
    }

    hr = D3DCompile(
        shr->data_fcode.str,
        shr->data_fcode.len,
        nullptr,
        nullptr,
        nullptr,
        shr->fragmentEntryName,
        "ps_5_0",
        0,
        0,
        &self->pixelBlob,
        &errors
    );

    if (FAILED(hr))
    {
        ReportHRError((const char*)errors->GetBufferPointer(), hr);
        return;
    }

    sgl_DXDevice* device = (sgl_DXDevice*)shr->gpu;
    hr = device->device->CreateVertexShader(
        self->vertexBlob->GetBufferPointer(),
        self->vertexBlob->GetBufferSize(),
        nullptr,
        &self->vertexShader
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to create vertex shader", hr);
        return;
    }

    hr = device->device->CreatePixelShader(
        self->pixelBlob->GetBufferPointer(),
        self->pixelBlob->GetBufferSize(),
        nullptr,
        &self->pixelShader
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to create pixel shader", hr);
        return;
    }

    std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
    elements.resize(shr->layout->elementCount);

    int semanticCounts[4] = { 0, 0, 0, 0 }; // indexed by sgl_VertexElementSemantic

    for (uint32 i = 0; i < shr->layout->elementCount; ++i)
    {
        D3D11_INPUT_ELEMENT_DESC& element = elements[i];
        sgl_VertexElement& src = shr->layout->elements[i];

        element.SemanticName = ToSemanticName(src.semantic);
        element.SemanticIndex = semanticCounts[src.semantic]++;
        element.Format = ToDXGIFormat(src.type);
        element.InputSlot = src.perInstance ? 1 : 0;
        element.AlignedByteOffset = (UINT)src.offset;
        element.InputSlotClass = src.perInstance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
        element.InstanceDataStepRate = src.perInstance ? 1 : 0;
    }

    hr = device->device->CreateInputLayout(elements.data(), (UINT)elements.size(), self->vertexBlob->GetBufferPointer(), self->vertexBlob->GetBufferSize(), &self->inputLayout);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create input layout", hr);
        return;
    }

    sgl_FreeString(shr->data_vcode);
    sgl_FreeString(shr->data_fcode);

    shr->gpuLoaded = true;
}

static void DXBind(sgl_Shader* shr)
{
    GetSelf;

    if (!shr->gpuLoaded)
        DXInitialise(shr);

    sgl_DXDevice* device = (sgl_DXDevice*)shr->gpu;

    device->ctx->VSSetShader(
        self->vertexShader,
        nullptr,
        0
    );

    device->ctx->PSSetShader(
        self->pixelShader,
        nullptr,
        0
    );

    device->ctx->IASetInputLayout(self->inputLayout);
}

static void DXDestroy(sgl_Shader* shr)
{
    GetSelf;

    if (self->vertexShader)
        self->vertexShader->Release();

    if (self->pixelShader)
        self->pixelShader->Release();

    if (self->vertexBlob)
        self->vertexBlob->Release();

    if (self->pixelBlob)
        self->pixelBlob->Release();

    if (self->inputLayout)
        self->inputLayout->Release();

    sgl::Memory::Delete(self);
}

static const sgl_ShaderVTable gDXVTable =
{
    .Bind = &DXBind,
    .Destroy = &DXDestroy,
};

sgl_DXShader* sgl_DXShader_Create()
{
    sgl_DXShader* shader = sgl::Memory::New<sgl_DXShader>();

    shader->base.gpuLoaded = false;
    shader->base.data_vcode = { .str = nullptr, .len = 0, .capacity = 0 };
    shader->base.data_fcode = { .str = nullptr, .len = 0, .capacity = 0 };
    shader->base.vtable = &gDXVTable;

    return shader;
}

#endif