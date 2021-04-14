#pragma once

#include "InputHandler.h"
#include "Graphics.h"
#include "RenderPass.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include <vector>

class Scene
{
private:
	Camera camera;
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<RenderPass>> renderPasses;
public:
	Scene() = default;
	Scene(Graphics& graphics, UINT windowWidth, UINT windowHeight, HWND window);
	bool AddModel(Graphics& graphics, const std::string& path);
	void AddLight();
	void Update(InputHandler& input ,float dt);
	void Render(Graphics& graphics);

	const std::vector<std::shared_ptr<Light>>& GetLights() const	{ return this->lights; };
	const std::vector<std::shared_ptr<Model>>& GetModels() const	{ return this->models; };
	const Camera& GetCamera() const									{ return this->camera; };
};