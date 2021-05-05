#pragma once

#include "Importer.h"
#include "Graphics.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "Scenario.h"
#include "ParticleSystem.h"
#include <vector>
#include "ShaderData.h"
#include "ParticleShader.h"

//IDs / sorterad lista (olika shaders, konstanter f�r framen en g�ng)

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
	Shader sh;
	ShaderData shaderData;
	ParticleShader particleShader;

	void AddParticleSystem(EmitterType emitterType, XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
public:
	Scene() = default;
	Scene(UINT windowWidth, UINT windowHeight, HWND window);
	bool AddModel(const std::string& path);
	void AddLight();
	void Update(InGameUI& ui, float dt);
	void Render();

	const std::vector<std::shared_ptr<Light>>& GetLights() const					{ return this->lights; }
	const std::vector<std::shared_ptr<Model>>& GetModels() const					{ return this->models; }
	const std::vector<std::shared_ptr<ParticleSystem>>& GetParticleSystems() const	{ return this->particleSystems; }
	const Camera& GetCamera() const													{ return this->camera; }
};