#pragma once
#include "Camera.h"
#include "Graphics.h"

#include <directXmath.h>
using namespace DirectX;

class ShaderData
{
	friend class Shader;
	friend class ParticleShader;
private:
	//-----GENERAL-----
	//CAMERA
	XMFLOAT3 cameraPosition = {};
	XMMATRIX viewMatrix = {};
	XMMATRIX projectionMatrix = {};

	//-----PARTICLE SYSTEM-----
	//BUFFERS
	ComPtr<ID3D11Buffer> viewProjBuffer;

	//SHADERS
	ComPtr<ID3D11VertexShader> particleVertexShader;
	ComPtr<ID3D11PixelShader> particlePixelShader;

	//MISC
	ComPtr<ID3D11InputLayout> particleLayout;

public:
	ShaderData();
	void Update(const Camera& camera);
};