#include "Scene.h"

Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, 0.001f, 40.0f, { 0, 20, 0 })
{
	Importer::LoadScene("Models/Test4.mff");
	Importer::LoadScene("Models/Office.mff");
	Importer::Initialize(Graphics::GetDevice());

	for (int i = 0; i < Importer::Data::scenes.size(); ++i)
	{
		for (auto& mesh : Importer::Data::GetMeshes(i))
		{
			auto model = std::make_shared<Model>(mesh);
			models.insert(std::make_pair(model->GetName(), model));
		}
	}

	//AddParticleSystem({ 50,50,50 }, { 60, 50,80 }, 50, 1, 100, 200, 0.2f);
	AddLight();

	scenario = Scenario(*this);
}

//void Scene::AddParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size)
//{
//	auto particleSystem = std::make_shared<ParticleSystem>(bounds, center, velocity, velocityVariation, particlesPerSecond, maxParticles, size);
//	particleSystems.push_back(particleSystem);
//}

void Scene::AddModel(std::shared_ptr<Model> model)
{
	models.insert(std::make_pair(model->GetName(), model));
}

void Scene::AddLight()
{
	auto light = std::make_shared<Light>();
	lights.push_back(light);
	gameObjects.push_back(light);
}

void Scene::Update(InGameUI& ui, float dt)
{
	XMFLOAT3 lastPosition = camera.GetPosition();

	//for (auto& model : models)
	//{
	//	bool hit = false;

	//	switch (model->collidertype)
	//	{
	//	case ColliderType::BOX:
	//		hit = camera.CheckCollision(model->boundingbox);
	//		break;
	//	case ColliderType::SPHERE:
	//		break;
	//	}

	//	if (hit)
	//	{
	//		camera.SetPosition(lastPosition);
	//		XMFLOAT3 direction = { camera.GetPosition().x - model->GetPosition().x, 0.0f,
	//								camera.GetPosition().z - model->GetPosition().z };
	//		camera.PushBack(direction, dt);
	//	}
	//}

	for (auto& particleSystem : particleSystems)
		particleSystem->Update(dt, camera.GetPosition());

	scenario.Update(*this, ui, camera);
	camera.Update(dt);
	shaderData.Update(camera);
}

void Scene::Render()
{
	particleShader.Render(shaderData, *this);
	regularShader.Render(shaderData, *this);
}