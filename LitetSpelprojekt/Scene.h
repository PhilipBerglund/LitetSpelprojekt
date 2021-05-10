#pragma once

#include <map>
#include "Graphics.h"
#include "Model.h"
#include "Light.h"
#include "Scenario.h"
#include "RainSystem.h"
#include "SmokeSystem.h"
#include "ParticleShader.h"
#include "RegularShader.h"

class Scene
{
	friend class Scenario;
private:
	Scenario scenario;

	Camera camera;

	std::map<std::string, std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Light>> lights;
	//std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<RainSystem>> rainSystem;
	std::vector<std::shared_ptr<SmokeSystem>> smokeSystem;

	//SHADERS
	ShaderData shaderData;
	ParticleShader GSRainShader;
	RegularShader regularShader;

	//void AddParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
	void AddRainParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity);
	void AddSmokeParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity);
	void AddModel(std::shared_ptr<Model> model);
	void AddLight();
public:
	Scene() = default;
	Scene(UINT windowWidth, UINT windowHeight, HWND window);

	void Update(InGameUI& ui, float dt);
	void Render();

	const std::vector<std::shared_ptr<Light>>& GetLights() const					{ return this->lights; }
	const std::map<std::string, std::shared_ptr<Model>>& GetModels() const			{ return this->models; }
	const std::vector<std::shared_ptr<RainSystem>>& GetRainSystem() const			{ return this->rainSystem; }
	const Camera& GetCamera() const													{ return this->camera; }
};