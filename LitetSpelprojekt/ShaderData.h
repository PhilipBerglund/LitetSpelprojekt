#pragma once
#include "Camera.h"
#include "Graphics.h"

#include <directXmath.h>
using namespace DirectX;

class ShaderData
{
	friend class Shader;
private:
	//-----GENERAL-----
	//CAMERA
	XMFLOAT3 cameraPosition;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	//-----PARTICLES-----
	//BUFFERS
	ComPtr<ID3D11Buffer> viewProj;

	//SHADERS
	ComPtr<ID3D11VertexShader> particleVertexShader;
	ComPtr<ID3D11PixelShader> particlePixelShader;

	//MISC
	ComPtr<ID3D11InputLayout> particleLayout;

public:
	void Update();
};