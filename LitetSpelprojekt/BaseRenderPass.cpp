#include "BaseRenderPass.h"

void BaseRenderPass::Execute(const Scene& scene, Graphics& graphics)
{
	const auto& models = scene.GetModels();
	const auto& camera = scene.GetCamera();
	const auto& lights = scene.GetLights();

	graphics.SetViewPerpsective(camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());

	for (int i = 0; i < models.size(); ++i)
		graphics.RenderModel(*models[i], *lights[0]);
}