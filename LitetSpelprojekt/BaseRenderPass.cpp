#include "BaseRenderPass.h"

void BaseRenderPass::Execute(const Scene& scene, Graphics& graphics)
{
	const auto& models = scene.GetModels();
	const auto& camera = scene.GetCamera();
	const auto& lights = scene.GetLights();

	XMMATRIX viewMatrix = camera.GetViewMatrix();
	XMMATRIX perspectiveMatrix = camera.GetPerspectiveMatrix();
	XMFLOAT3 cameraPosition = camera.GetPosition();

	shader.SetShader(graphics.GetDeviceContext());

	for (int i = 0; i < models.size(); ++i)
		shader.Render(graphics.GetDeviceContext(), *models[i], *lights[0], viewMatrix, perspectiveMatrix, cameraPosition);
}