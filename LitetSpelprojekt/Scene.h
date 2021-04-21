#pragma once

#include "InputHandler.h"
#include "Graphics.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "Scenario.h"
#include <vector>

//IDs / sorterad lista (olika shaders, konstanter för framen en gång)

class Scene
{
private:
	Scenario scenario;
	Camera camera;
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	Shader sh;
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