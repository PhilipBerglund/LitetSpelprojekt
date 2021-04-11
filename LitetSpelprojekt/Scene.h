#pragma once

#include "InputHandler.h"
#include "Graphics.h"
#include "RenderPass.h"
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
	Scene(Graphics& graphics, UINT windowWidth, UINT windowHeight);
	void AddModel(Graphics& graphics, const std::string& path);
	void AddLight();
	void Update(InputHandler& input ,float dt);
	void Render(Graphics& graphics);

	const std::vector<std::shared_ptr<Light>>& GetLights() const;
	const std::vector<std::shared_ptr<Model>>& GetModels() const;
	const Camera& GetCamera() const;
};