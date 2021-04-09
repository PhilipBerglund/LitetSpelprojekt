#include "Graphics.h"
#include "PipelineHelper.h"
#include "Camera.h"

Graphics::Graphics()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->layout = nullptr;
}

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!core.Initialize(windowWidth, windowHeight, window))
		return false;

	if (!shader.Initialize(core.GetDevice(), window))
		return false;

	return true;
}

void Graphics::ShutDown()
{
	core.ShutDown();

	shader.ShutDown();

	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}

	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}
}

void Graphics::Render(std::vector<GameObject*> gameObjects)
{
	core.BeginScene(1, 1, 1);
	Light light;

	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX perspectiveMatrix = XMMatrixIdentity();

	for (int i = 0; i < gameObjects.size(); ++i)
	{
		switch (gameObjects[i]->type())
		{
		case Type::CAMERA:
			viewMatrix = dynamic_cast<Camera*>(gameObjects[i])->GetViewMatrix();
			perspectiveMatrix = dynamic_cast<Camera*>(gameObjects[i])->GetPerspectiveMatrix();
			break;

		case Type::LIGHT:
			break;

		case Type::MODEL:
			shader.SetShader(core.GetDeviceContext());
			shader.Render(core.GetDeviceContext(), *dynamic_cast<Model*>(gameObjects[i]), light, viewMatrix, perspectiveMatrix);
			break;
		}
	}

	core.EndScene();
}

ID3D11Device* Graphics::GetDevice()
{
	return core.GetDevice();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return core.GetDeviceContext();
}
