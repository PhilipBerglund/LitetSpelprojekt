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

	//STRUCTS N STUFF

	ID3D11Buffer* VS_Buffer;
	struct VS
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 WVPMatrix;
		XMFLOAT4X4 light_WVP;
	};

	bool UpdateBuffers(ID3D11DeviceContext* context, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix);
public:
	Shader();
	void ShutDown();
	bool Initialize(ID3D11Device* device, HWND window);
	void SetShader(ID3D11DeviceContext* context, ID3D11ShaderResourceView* depthSRV);
	void Render(ID3D11DeviceContext* context, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix);
};