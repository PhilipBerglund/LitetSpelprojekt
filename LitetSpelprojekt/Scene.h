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
#include "ShadowMap.h"
#include "ShadowMapShader.h"
#include "Bounds.h"
#include "SoundHandler.h"
#include "QuadTree.h"

class Scene
{
	friend class Scenario;
private:
	Scenario scenario;
	Bounds bounds;
	Camera camera;
	QuadTree* tree;
	QTFrustum frust;

	std::map<std::string, std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Model>> QTModels;
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::vector<std::shared_ptr<RainSystem>> rainSystem;
	std::vector<std::shared_ptr<SmokeSystem>> smokeSystem;
	std::vector<std::shared_ptr<ShadowMap>> shadowMaps;
	std::map<std::string, std::shared_ptr<Model>> nonShadowModels;

	//AUDIO
	SoundHandler soundHandler;

	//SHADERS
	ShaderData shaderData;
	ParticleShader GSRainShader;
	ParticleShader GSSmokeShader;
	RegularShader regularShader;
	ShadowMapShader ShadowMapShader;

	void AddRainParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity);
	void AddSmokeParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity, XMFLOAT4 starPos, float maxParticleRange);
	void AddModel(std::shared_ptr<Model> model);
	void AddLight();
	void AddShadowMap(float width, float height);
public:
	Scene() = default;
	Scene(UINT windowWidth, UINT windowHeight, HWND window);
	
	void Reset(InGameUI& ui);
	void Update(InGameUI& ui, float dt);
	void Render();
	void RenderShadowMap();
	void ClearQTModels()															{ this->QTModels.clear(); }

	const std::vector<std::shared_ptr<Model>>& GetQTModels() const					{ return this->QTModels; }
	const std::vector<std::shared_ptr<Light>>& GetLights() const					{ return this->lights; }
	const std::map<std::string, std::shared_ptr<Model>>& GetModels() const			{ return this->models; }
	const std::map<std::string, std::shared_ptr<Model>>& GetNoShadowModels() const	{ return this->nonShadowModels; }
	const std::vector<std::shared_ptr<RainSystem>>& GetRainSystem() const			{ return this->rainSystem; }
	const std::vector<std::shared_ptr<SmokeSystem>>& GetSmokeSystem() const			{ return this->smokeSystem; }
	const Camera& GetCamera() const													{ return this->camera; }
};