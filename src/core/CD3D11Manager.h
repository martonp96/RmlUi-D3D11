#pragma once
#include "utils/CSingleton.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <PostProcess.h>

class CD3D11Manager : public CSingleton<CD3D11Manager>
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtw;

	ID3D11BlendState* alphaBS;
	ID3D11RasterizerState* rs;
	ID3D11RasterizerState* scissoredRs;
	ID3D11DepthStencilState* depthStencil;
	ID3D11DepthStencilView* mainDSV;

	ID3D11Texture2D* m_OffScreenTexture;
	ID3D11RenderTargetView* m_OffScreenRTV;
	ID3D11ShaderResourceView* m_OffScreenSRV;

	std::shared_ptr<DirectX::BasicPostProcess> m_BasicPostProcess;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;

	int width, height;
public:
	void Create();
	void PreRender();
	void Render();

	ID3D11DeviceContext* GetContext() { return context; }

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const char* shaderBuf, const char* entryPoint, const char* profile);
	void CreateShaders();
	void SetShaders();
	void CreateBS(DXGI_SWAP_CHAIN_DESC);
	void SetAlphaBS();
	Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(const char* data, size_t size, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, int cpuAccess);
	void EditBuffer(const char* data, size_t byteSize, ID3D11Buffer* bufferPointer);
	void DrawIndexed(UINT number);
	void SetTemporaryRS();
	void SetTemporaryScissoredRS();
	void SetScissorRectangle(int x, int y, int width, int height);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTextureFromFile(const char* imageFileData);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTextureFromPixels(const char* imageRawData, unsigned int width, unsigned int height);

	template <typename T>
	void EditBuffer(const T& data, ID3D11Buffer* bufferPointer);

	template <class T>
	Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(const T& data, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess);
};

template <typename T>
inline void CD3D11Manager::EditBuffer(const T& data, ID3D11Buffer* bufferPointer)
{
	EditBuffer((const char*)&data, sizeof(T), bufferPointer);
}

template <class T>
inline Microsoft::WRL::ComPtr<ID3D11Buffer> CD3D11Manager::CreateBuffer(const T& data, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpuAccess)
{
	return CreateBuffer((const char*)&data, sizeof(T), bindFlags, usage, cpuAccess);
}