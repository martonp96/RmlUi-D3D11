#pragma once

typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::SimpleMath::Vector2 Vector2;
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector4 Vector4;
typedef DirectX::SimpleMath::Quaternion Quaternion;
typedef DirectX::SimpleMath::Ray Ray;
typedef DirectX::BoundingBox BoundingBox;
typedef DirectX::SimpleMath::Color Color;

struct VertexData
{
	Vector3 position;
	Vector3 normal;
	Vector2 textureCoord;
	Vector3 tangent;
};

struct InstanceData
{
	Matrix transform;
	Matrix inverseTransposeTransform;
	Color color;

	InstanceData() = default;
	InstanceData(const Matrix& matrix, const Color& instanceColor)
	{
		transform = matrix;
		inverseTransposeTransform = matrix.Invert().Transpose();
		color = instanceColor;
	}
};

struct UIVertexData
{
	Vector2 position;
	char color[4];
	Vector2 textureCoord;
};

struct AnimatedVertexData : public VertexData
{
	int boneIndices[4];
	Vector4 boneWeights;
};

struct FXAAData
{
	Vector3 position;
	Vector2 texturecoord;
};

class Texture
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv_texture;
public:
	Texture(const char* source, int width, int height) : srv_texture(nullptr)
	{
		srv_texture = CD3D11Manager::Get()->CreateTextureFromPixels(source, width, height).Get();
	}

	Texture(const char* source) : srv_texture(nullptr)
	{
		srv_texture = CD3D11Manager::Get()->CreateTextureFromFile(source).Get();
	}

	~Texture()
	{
		if (srv_texture)
		{
			srv_texture->Release();
			srv_texture = nullptr;
		}
	}

	ID3D11ShaderResourceView* getShaderResourceView()
	{
		return srv_texture.Get();
	}
};
