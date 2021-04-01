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
	std::string vs_path;
	ID3D11VertexShader* vertexShader;

	std::string ps_path;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;

	ID3D11Buffer* VS_Buffer;

	//STRUCTS N STUFF
	struct VS
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 WVPMatrix;
		XMFLOAT4X4 light_WVP;
	};

public:
	Shader();
	void ShutDown();
	bool Initialize(ID3D11Device* device, HWND window);
	void Render(ID3D11DeviceContext* context, ID3D11ShaderResourceView* depthSRV, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix);
};