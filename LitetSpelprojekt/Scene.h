#pragma once

#include "Graphics.h"
#include "Model.h"
#include "Light.h"
#include "Scenario.h"
#include "ParticleSystem.h"
#include "ParticleShader.h"
#include "RegularShader.h"

class Scene
{
private:
	Scenario scenario;

	Camera camera;

	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<ParticleSystem>> particleSystems;

	//SHADERS
	ShaderData shaderData;
	ParticleShader particleShader;
	RegularShader regularShader;

	void AddParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
	bool AddModel(const std::string& path);
	void AddLight();
public:
	Scene() = default;
	Scene(UINT windowWidth, UINT windowHeight, HWND window);

	void Update(InGameUI& ui, float dt);
	void Render();

	const std::vector<std::shared_ptr<Light>>& GetLights() const					{ return this->lights; }
	const std::vector<std::shared_ptr<Model>>& GetModels() const					{ return this->models; }
	const std::vector<std::shared_ptr<ParticleSystem>>& GetParticleSystems() const	{ return this->particleSystems; }
	const Camera& GetCamera() const													{ return this->camera; }
};