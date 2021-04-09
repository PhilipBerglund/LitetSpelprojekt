#pragma once
#include "DirectXEssentials.h"
#include "Model.h"
#include <iostream>
#include "Shader.h"

class Graphics
{
private:
	DirectXEssentials core;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	Shader shader;
public:
	Graphics();
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);
	void ShutDown();
	void Render(std::vector<GameObject*> gameObjects);
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
};