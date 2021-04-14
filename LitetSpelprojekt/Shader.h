#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

#include "ComPtr.h"
#include "Model.h"
#include "Light.h"

using namespace DirectX;

class Shader
{
private:
	std::string vs_path = "../x64/Debug/VertexShader.cso";
	std::string ps_path = "../x64/Debug/PixelShader.cso";

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> layout;

	ComPtr<ID3D11Buffer> VS_Buffer;
	struct VS
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 perspectiveMatrix;

		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 lightPerspectiveMatrix;
	};

	ComPtr<ID3D11Buffer> PS_Buffer;
	struct PS
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	bool UpdateBuffers(ID3D11DeviceContext& context, const Model& model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix, XMFLOAT3 cameraPosition);
public:
	Shader() = default;
	bool Initialize(ID3D11Device& device, HWND window);
	void SetShader(ID3D11DeviceContext& context);
	void Render(ID3D11DeviceContext& context, const Model& model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix, XMFLOAT3 cameraPosition);
};