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
public:
	Graphics() = default;
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginFrame();
	void EndFrame();

	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();
};