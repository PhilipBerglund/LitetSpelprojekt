#pragma once
#include "DirectXEssentials.h"
#include "Model.h"
#include <iostream>
#include "Shader.h"
#include "Camera.h"

class Graphics
{
private:
	DirectXEssentials core;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> layout;

	Shader shader;

	XMMATRIX viewMatrix;
	XMMATRIX perspectiveMatrix;
	XMFLOAT3 cameraPosision;
public:
	Graphics() = default;
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);
	void Render(const std::vector<GameObject*>& gameObjects, Camera* camera);

	void SetViewPerpsective(XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix, XMFLOAT3 position);
	void RenderModel(const Model& model, const Light& light);
	void RenderParticleSystem();
	void BeginRendering();
	void EndRendering();

	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();
};