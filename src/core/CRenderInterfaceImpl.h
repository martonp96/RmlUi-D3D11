#pragma once
#include "utils/CSingleton.h"
#include <RmlUi/Core.h>
#include "SimpleMath.h"
#include "utils/TextureStuff.h"

class CRenderInterfaceImpl : public Rml::RenderInterface, public CSingleton<CRenderInterfaceImpl>
{
	struct GeometryData
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		unsigned int vertexStride;
		unsigned int vertexCount;

		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		unsigned int indexCount;
		DXGI_FORMAT indexFormat;

		Rml::TextureHandle textureHandle;

		GeometryData(const UIVertexData* vertices, size_t verticesSize, int* indices, size_t indicesSize, Rml::TextureHandle texture) 
			: vertexBuffer(nullptr), vertexStride(0), vertexCount(0), textureHandle(texture), indexBuffer(nullptr), indexCount(0), indexFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)
		{
			vertexBuffer = CD3D11Manager::Get()->CreateBuffer((const char*)vertices, sizeof(UIVertexData)*verticesSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
			vertexStride = sizeof(UIVertexData);
			vertexCount = verticesSize;

			indexBuffer = CD3D11Manager::Get()->CreateBuffer((const char*)indices, sizeof(int) * indicesSize, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
			indexCount = indicesSize;
			indexFormat = DXGI_FORMAT_R32_UINT;
		}
	};

	uint32_t textureCount;

	Matrix m_UITransform;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ModelMatrixBuffer = nullptr;
	std::unordered_map<unsigned int, std::shared_ptr<Texture>> textures;

	Texture* whiteTexture;

	int width, height;

	void Render(GeometryData* geometry, const Rml::Vector2f& translation);

	virtual void RenderGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

	virtual Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::TextureHandle texture) override;
	virtual void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;
	virtual void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

	virtual bool LoadTexture(Rml::TextureHandle& textureHandle, Rml::Vector2i& textureDimensions, const Rml::String& source) override;
	virtual bool GenerateTexture(Rml::TextureHandle& textureHandle, const uint8_t* source, const Rml::Vector2i& sourceDimensions) override;
	virtual void ReleaseTexture(Rml::TextureHandle texture);

	virtual void EnableScissorRegion(bool enable) override;
	virtual void SetScissorRegion(int x, int y, int width, int height) override;

	virtual void SetTransform(const Rml::Matrix4f* transform) override;
public:
	void Init();
};