#include "Graphics.h"
#include "PipelineHelper.h"

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!core.Initialize(windowWidth, windowHeight, window))
		return false;

	if (!shader.Initialize(core.GetDevice(), window))
		return false;

	return true;
}

void Graphics::Render(const std::vector<GameObject*>& gameObjects, Camera* camera)
{
	core.BeginScene(1, 1, 1);

	core.EndScene();
}

void Graphics::SetViewPerpsective(XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix, XMFLOAT3 position)
{
	this->viewMatrix = viewMatrix;
	this->perspectiveMatrix = perspectiveMatrix;
	this->cameraPosision = position;
}

void Graphics::RenderModel(const Model& model, const Light& light)
{
	shader.SetShader(core.GetDeviceContext());
	shader.Render(core.GetDeviceContext(), model, light, viewMatrix, perspectiveMatrix, cameraPosision);
}

void Graphics::BeginRendering()
{
	core.BeginScene(1, 1, 1);
}

void Graphics::EndRendering()
{
	core.EndScene();
	viewMatrix = XMMatrixIdentity();
	perspectiveMatrix = XMMatrixIdentity();
}

ID3D11Device& Graphics::GetDevice()
{
	return core.GetDevice();
}

ID3D11DeviceContext& Graphics::GetDeviceContext()
{
	return core.GetDeviceContext();
}