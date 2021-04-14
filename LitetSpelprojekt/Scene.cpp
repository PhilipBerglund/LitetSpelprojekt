#include "Scene.h"
#include "BaseRenderPass.h"

Scene::Scene(Graphics& graphics, UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f, { 0,0,-10 })
{
	renderPasses.push_back(std::make_unique<BaseRenderPass>(graphics.GetDevice(), window));

	AddModel(graphics, "Models/Troll.obj");
	AddLight();
}

bool Scene::AddModel(Graphics& graphics, const std::string& path)
{
	auto model = std::make_shared<Model>();
	models.push_back(model);
	gameObjects.push_back(model);
	if (!model->Initialize(graphics.GetDevice(), path))
	{
		Error("- FAILED TO INITIALIZE MODEL -");
		return false;
	}

	return true;
}

void Scene::AddLight()
{
	auto light = std::make_shared<Light>();
	lights.push_back(light);
	gameObjects.push_back(light);
}

void Scene::Update(InputHandler& input, float dt)
{
	camera.Rotate((float)input.ReadRawDelta().value().x, (float)input.ReadRawDelta().value().y);

	if (input.KeyIsPressed('W'))
		camera.MoveForward(dt);

	if (input.KeyIsPressed('S'))
		camera.MoveForward(-dt);

	if (input.KeyIsPressed('D'))
		camera.MoveRight(dt);

	if (input.KeyIsPressed('A'))
		camera.MoveRight(-dt);

	camera.Update();
}

void Scene::Render(Graphics& graphics)
{
	for (auto& renderPass : renderPasses)
		renderPass->Execute(*this, graphics);
}