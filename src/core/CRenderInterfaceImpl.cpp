#include "main.h"

#define PER_OBJECT_VS_CPP 3
#define DIFFUSE_PS_CPP 1

struct PerUIVSCB
{
	Matrix Model;
	PerUIVSCB() = default;
	PerUIVSCB(const Matrix& model)
	{
		Model = model.Transpose();
	}
};

void CRenderInterfaceImpl::Init()
{ 
    width = CAppManager::Get()->GetWidth();
    height = CAppManager::Get()->GetHeight();

    m_ModelMatrixBuffer = CD3D11Manager::Get()->CreateBuffer<PerUIVSCB>(PerUIVSCB(), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

    char whiteBuf[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    textures[0] = std::make_shared<Texture>(whiteBuf, 1, 1);
}

void CRenderInterfaceImpl::Render(GeometryData* geometry, const Rml::Vector2f& translation)
{
    auto deviceMgr = CD3D11Manager::Get();

    const UINT offset = 0u;
    deviceMgr->GetContext()->IASetVertexBuffers(0u, 1, geometry->vertexBuffer.GetAddressOf(), &geometry->vertexStride, &offset);
    deviceMgr->GetContext()->IASetIndexBuffer(geometry->indexBuffer.Get(), geometry->indexFormat, 0u);

    deviceMgr->SetShaders();

    auto shit = PerUIVSCB(Matrix::CreateTranslation(translation.x, translation.y, 0.0f) * m_UITransform * Matrix::CreateOrthographic(width, height, 0.0f, 10000.0f));

    deviceMgr->EditBuffer(shit, m_ModelMatrixBuffer.Get());
    deviceMgr->GetContext()->VSSetConstantBuffers(PER_OBJECT_VS_CPP, 1, m_ModelMatrixBuffer.GetAddressOf());

    ID3D11ShaderResourceView* textt[] = { textures[geometry->textureHandle]->getShaderResourceView() };        
    deviceMgr->GetContext()->PSSetShaderResources(DIFFUSE_PS_CPP, 1, textt);

    deviceMgr->GetContext()->DrawIndexed(geometry->indexCount, 0u, 0u);
}

void CRenderInterfaceImpl::RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
    printf("RenderGeometry\n");
	GeometryData geometry((UIVertexData*)vertices, numVertices, (int*)indices, numIndices, texture);
	Render(&geometry, translation);
}

Rml::CompiledGeometryHandle CRenderInterfaceImpl::CompileGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture)
{
	return (Rml::CompiledGeometryHandle) new GeometryData((UIVertexData*)vertices, numVertices, (int*)indices, numIndices, texture);
}

void CRenderInterfaceImpl::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation)
{
    //printf("RenderCompiledGeometry\n");
	Render((GeometryData*)geometry, translation);
}

void CRenderInterfaceImpl::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
    printf("ReleaseCompiledGeometry\n");
	delete (GeometryData*)geometry;
}

bool CRenderInterfaceImpl::LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source)
{
    printf("LoadTexture %s\n", source.c_str());

    textureHandle = ++textureCount;
    textures[textureHandle] = std::make_shared<Texture>(source.c_str());

    return true;
}

bool CRenderInterfaceImpl::GenerateTexture(Rml::TextureHandle& textureHandle, const byte* source, const Rml::Vector2i& sourceDimensions)
{
    printf("GenerateTexture\n");

    textureHandle = ++textureCount;
    textures[textureHandle].reset(new Texture((const char*)source, sourceDimensions.x, sourceDimensions.y));
    return textures[textureHandle] != nullptr;
}

void CRenderInterfaceImpl::ReleaseTexture(Rml::TextureHandle texture)
{
    printf("ReleaseTexture\n");
    textures[texture].reset();
}

void CRenderInterfaceImpl::EnableScissorRegion(bool enable)
{
	enable ? CD3D11Manager::Get()->SetTemporaryScissoredRS() : CD3D11Manager::Get()->SetTemporaryRS();
}

void CRenderInterfaceImpl::SetScissorRegion(int x, int y, int _width, int _height)
{
	CD3D11Manager::Get()->SetScissorRectangle(x, y, _width, _height);
}

void CRenderInterfaceImpl::SetTransform(const Rml::Matrix4f* transform)
{
	if (!transform)
	{
		m_UITransform = Matrix::Identity;
		return;
	}
	m_UITransform = Matrix(transform->data());
}