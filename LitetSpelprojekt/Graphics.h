#pragma once
#include "DirectXEssentials.h"
#include "Model.h"
#include <iostream>

class Graphics
{
private:
	DirectXEssentials core;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampler;
public:
	Graphics();
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);
	void ShutDown();
	void Render();
};