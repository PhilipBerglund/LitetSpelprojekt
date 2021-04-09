#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include "Model.h"
#include "Light.h"

using namespace DirectX;

class Shader
{
private:
	std::string vs_path = "../x64/Debug/VertexShader.cso";
	std::string ps_path = "../x64/Debug/PixelShader.cso";

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	ID3D11Buffer* VS_Buffer;
	struct VS
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 perspectiveMatrix;

		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 lightPerspectiveMatrix;
	};

	bool UpdateBuffers(ID3D11DeviceContext* context, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix);
public:
	Shader();
	void ShutDown();
	bool Initialize(ID3D11Device* device, HWND window);
	void SetShader(ID3D11DeviceContext* context);
	void Render(ID3D11DeviceContext* context, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix);
};