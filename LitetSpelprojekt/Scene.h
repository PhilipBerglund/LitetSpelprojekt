#pragma once

#include "Importer.h"
#include "InputHandler.h"
#include "Graphics.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "Scenario.h"
#include "ParticleSystem.h"
#include <vector>

//IDs / sorterad lista (olika shaders, konstanter f�r framen en g�ng)

class Scene
{
private:
	Scenario scenario;
	Camera camera;
	ParticleSystem rainSystem;
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	Shader sh;
public:
	Scene() = default;
	Scene(UINT windowWidth, UINT windowHeight, HWND window);
	bool AddModel(const std::string& path);
	void AddLight();
	void Update(InGameUI& ui, float dt);
	void Render();

	const std::vector<std::shared_ptr<Light>>& GetLights() const	{ return this->lights; };
	const std::vector<std::shared_ptr<Model>>& GetModels() const	{ return this->models; };
	const Camera& GetCamera() const									{ return this->camera; };
};