#include "main.h"
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include "WICTextureLoader.h"
#include <iostream>
#include <fstream>
#include "shaders/shaders.h"

void CD3D11Manager::Create()
{
    width = CAppManager::Get()->GetWidth();
    height = CAppManager::Get()->GetHeight();

    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    sd.BufferCount = 2;
    sd.OutputWindow = CAppManager::Get()->GetWindow();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &context) != S_OK)
    {
        CAppManager::Get()->Msg("Failed to create d3d11 device!");
    }
    
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    context->RSSetViewports(1u, &vp);

    ID3D11Texture2D* pBackBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    device->CreateRenderTargetView(pBackBuffer, NULL, &rtw);
    pBackBuffer->Release();

    CreateShaders();
    CreateBS(sd);
}

void CD3D11Manager::CreateBS(DXGI_SWAP_CHAIN_DESC sd)
{
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    D3D11_RENDER_TARGET_BLEND_DESC renderBlendDesc;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, &alphaBS);

    {
        D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        device->CreateDepthStencilState(&dsDesc, &depthStencil);
        context->OMSetDepthStencilState(depthStencil, 1u);
        //m_StencilRef = 1u;
    }

    {
        D3D11_RASTERIZER_DESC rsDesc;
        rsDesc.FillMode = D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_NONE;
        rsDesc.FrontCounterClockwise = FALSE;
        rsDesc.DepthBias = 0;
        rsDesc.SlopeScaledDepthBias = 0.0f;
        rsDesc.DepthBiasClamp = 0.0f;
        rsDesc.DepthClipEnable = FALSE;
        rsDesc.ScissorEnable = FALSE;
        rsDesc.MultisampleEnable = FALSE;
        rsDesc.AntialiasedLineEnable = FALSE;

        device->CreateRasterizerState(&rsDesc, &rs);
    }

    {
        D3D11_RASTERIZER_DESC rsDesc;
        rsDesc.FillMode = D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_NONE;
        rsDesc.FrontCounterClockwise = FALSE;
        rsDesc.DepthBias = 0;
        rsDesc.SlopeScaledDepthBias = 0.0f;
        rsDesc.DepthBiasClamp = 0.0f;
        rsDesc.DepthClipEnable = TRUE;
        rsDesc.ScissorEnable = TRUE;
        rsDesc.MultisampleEnable = FALSE;
        rsDesc.AntialiasedLineEnable = FALSE;

        device->CreateRasterizerState(&rsDesc, &scissoredRs);
    }

    {
        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        device->CreateTexture2D(&textureDesc, NULL, &m_OffScreenTexture);

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        device->CreateRenderTargetView(m_OffScreenTexture, &renderTargetViewDesc, &m_OffScreenRTV);

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(m_OffScreenTexture, &shaderResourceViewDesc, &m_OffScreenSRV);
    }

    m_BasicPostProcess.reset(new DirectX::BasicPostProcess(device));
}

void CD3D11Manager::PreRender()
{
    context->OMSetRenderTargets(1, &rtw, nullptr);

    m_BasicPostProcess->SetSourceTexture(m_OffScreenSRV);
    m_BasicPostProcess->SetEffect(DirectX::BasicPostProcess::Effect::Copy);
    m_BasicPostProcess->Process(context);

    SetAlphaBS();
}

void CD3D11Manager::Render()
{
    swapChain->Present(0, 0);
}

void CD3D11Manager::EditBuffer(const char* data, size_t byteSize, ID3D11Buffer* bufferPointer)
{
    D3D11_MAPPED_SUBRESOURCE subresource;
    context->Map(bufferPointer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &subresource);
    memcpy(subresource.pData, data, byteSize);
    context->Unmap(bufferPointer, 0); 
}

void CD3D11Manager::DrawIndexed(UINT number)
{
    //printf("DrawIndexed %d\n", number);
    context->DrawIndexed(number, 0u, 0u);
}

void CD3D11Manager::SetTemporaryRS()
{
    context->RSSetState(rs);
}

void CD3D11Manager::SetTemporaryScissoredRS()
{
    context->RSSetState(scissoredRs);
}

void CD3D11Manager::SetScissorRectangle(int x, int y, int _width, int _height)
{
    D3D11_RECT rect;
    rect.left = x;
    rect.right = x + _width;
    rect.top = y;
    rect.bottom = y + _height;

    context->RSSetScissorRects(1, &rect);
}

void CD3D11Manager::SetShaders()
{
    context->VSSetShader(vs.Get(), nullptr, 0u);
    context->PSSetShader(ps.Get(), nullptr, 0u);
    context->IASetInputLayout(il.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CD3D11Manager::SetAlphaBS()
{
    static float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->OMSetBlendState(alphaBS, blendFactors, 0xffffffff);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CD3D11Manager::CreateTextureFromFile(const char* path)
{
    Microsoft::WRL::ComPtr<ID3D11Resource> textureResource;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;

    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

    auto size = file.tellg();
    auto imageFileData = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(imageFileData, size);
    file.close();

    auto hr = DirectX::CreateWICTextureFromMemoryEx(device, (const uint8_t*)imageFileData, size, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB | DirectX::WIC_LOADER_FORCE_RGBA32, textureResource.GetAddressOf(), textureView.GetAddressOf());

    if (FAILED(hr))
    {
        CAppManager::Get()->Msg("Could not create texture from file data: 0x%08x", hr);
    }

    return textureView;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CD3D11Manager::CreateTextureFromPixels(const char* imageRawData, unsigned int _width, unsigned int _height)
{
    D3D11_TEXTURE2D_DESC textureDesc = {};

    textureDesc.Width = _width;
    textureDesc.Height = _height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = imageRawData;
    data.SysMemPitch = _width * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
    auto hr = device->CreateTexture2D(&textureDesc, &data, &texture2D);
    if (FAILED(hr))
    {
        CAppManager::Get()->Msg("Could not create texture 2D from pixel data: 0x%08x", hr);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
    device->CreateShaderResourceView(texture2D.Get(), &srvDesc, &textureSRV);

    return textureSRV;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> CD3D11Manager::CreateBuffer(const char* data, size_t size, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, int cpuAccess)
{
    D3D11_BUFFER_DESC bd = { 0 };
    bd.BindFlags = bindFlags;
    bd.Usage = usage;
    bd.CPUAccessFlags = cpuAccess;
    bd.MiscFlags = 0u;
    bd.ByteWidth = size;
    bd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA sd = { 0 };
    sd.pSysMem = data;

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    device->CreateBuffer(&bd, &sd, &buffer);
    return buffer;
}

void CD3D11Manager::CreateShaders()
{
    auto psBlob = CompileShader(g_pixelShader, "main", "ps_4_0");
    auto vsBlob = CompileShader(g_vertexShader, "main", "vs_4_0");

    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);

    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDescArray;
    UINT offset = 0;

    D3D11_INPUT_ELEMENT_DESC desc;
    desc.SemanticName = "POSITION";
    desc.SemanticIndex = 0;
    desc.Format = DXGI_FORMAT_R32G32_FLOAT;
    desc.InputSlot = 0;
    desc.AlignedByteOffset = offset;
    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc.InstanceDataStepRate = 0;
    offset += sizeof(float) * 2;
    vertexDescArray.push_back(desc);

    desc.SemanticName = "COLOR";
    desc.SemanticIndex = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.InputSlot = 0;
    desc.AlignedByteOffset = offset;
    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc.InstanceDataStepRate = 0;
    offset += sizeof(char) * 4;
    vertexDescArray.push_back(desc);

    desc.SemanticName = "TEXCOORD";
    desc.SemanticIndex = 0;
    desc.Format = DXGI_FORMAT_R32G32_FLOAT;
    desc.InputSlot = 0;
    desc.AlignedByteOffset = offset;
    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc.InstanceDataStepRate = 0;
    offset += sizeof(float) * 2;
    vertexDescArray.push_back(desc);

    device->CreateInputLayout(
        vertexDescArray.data(), vertexDescArray.size(),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &il);

    //context->IASetInputLayout(il.Get());
}

Microsoft::WRL::ComPtr<ID3DBlob> CD3D11Manager::CompileShader(const char* shaderBuf, const char* entryPoint, const char* profile)
{
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG;
#else
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    const D3D_SHADER_MACRO defines[1] = { NULL, NULL };
    auto hr = D3DCompile(
            shaderBuf,
            strlen(shaderBuf),
            NULL,
            defines,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            profile,
            flags,
            0,
            &shaderBlob,
            &errorBlob);
    
    if (FAILED(hr))
    {
        CAppManager::Get()->Msg("Failed to create the shader: 0x%08x", hr);
    }

    if (errorBlob)
    {
        CAppManager::Get()->Msg("Error when compiling shader: %s", (char*)errorBlob->GetBufferPointer());
    }

    return shaderBlob;
}