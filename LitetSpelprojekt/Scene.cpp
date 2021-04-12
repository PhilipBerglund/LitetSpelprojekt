#include "Scene.h"
#include "BaseRenderPass.h"

Scene::Scene(Graphics& graphics, UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f, { 0,0,-20 })
{
	renderPasses.push_back(std::make_unique<BaseRenderPass>(graphics.GetDevice(), window));

	AddModel(graphics, "Models/Troll.obj");
	AddLight();
}

void Scene::AddModel(Graphics& graphics, const std::string& path)
{
	auto model = std::make_shared<Model>();
	models.push_back(model);
	gameObjects.push_back(model);
	model->Initialize(graphics.GetDevice(), path);
}

void Scene::AddLight()
{
	auto light = std::make_shared<Light>();
	lights.push_back(light);
	gameObjects.push_back(light);
}

void Scene::Update(InputHandler& input, float dt)
{
	if (input.KeyIsPressed('W'))
		camera.MoveForward(camera.GetSpeed() * dt);

	if (input.KeyIsPressed('S'))
		camera.MoveForward(camera.GetSpeed() * -dt);

	if (input.KeyIsPressed('D'))
		camera.MoveRight(camera.GetSpeed() * dt);

	if (input.KeyIsPressed('A'))
		camera.MoveRight(camera.GetSpeed() * -dt);

	camera.Update();
}

void Scene::Render(Graphics& graphics)
{
	graphics.BeginFrame();

	/*for (int i = 0; i < renderPasses.size(); ++i)
		renderPasses[i]->Execute(*this, graphics);*/

	for (auto& renderPass : renderPasses)
		renderPass->Execute(*this, graphics);

	graphics.EndFrame();
}

const std::vector<std::shared_ptr<Light>>& Scene::GetLights() const
{
	return this->lights;
}

const std::vector<std::shared_ptr<Model>>& Scene::GetModels() const
{
	return this->models;
}

const Camera& Scene::GetCamera() const
{
	return this->camera;
}