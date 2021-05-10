#include "Scene.h"

Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, 0.001f, 50.0f, { 0, 15, 0 })
{
	//Importer::LoadScene("Models/Dumpster.mff");
	//Importer::LoadScene("Models/Office.mff");
	Importer::LoadScene("Models/Bar.mff");
	Importer::LoadScene("Models/Hotel.mff");
	Importer::LoadScene("Models/Restaurant.mff");
	Importer::LoadScene("Models/Park.mff");
	Importer::LoadScene("Models/OutsideObjects.mff");
	Importer::LoadScene("Models/Library.mff");
	Importer::LoadScene("Models/StockHouses.mff");
	Importer::LoadScene("Models/Streets.mff");

	Importer::Initialize(Graphics::GetDevice());

	for (int i = 0; i < Importer::Data::scenes.size(); ++i)
	{
		for (auto& mesh : Importer::Data::GetMeshes(i))
		{
			auto model = std::make_shared<Model>(mesh);
			models.insert(std::make_pair(model->GetName(), model));
		}
	}

	AddGSParticleSystem(3000, 150, 200);
	AddLight();

	scenario = Scenario(*this);
}


void Scene::AddGSParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity)
{
	auto particleSystem = std::make_shared<RainSystem>(maxParticles, minVelocity, maxVelocity);
	rainSystem.push_back(particleSystem);
}

void Scene::AddModel(std::shared_ptr<Model> model)
{
	int dupes = 0;

	for (auto& mod : models)
	{
		if (mod.first == model->GetName())
			dupes++;
	}

	if (dupes != 0)
		model->SetName(model->GetName() + std::to_string(dupes));

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

	for (auto& particleSystem : rainSystem)
		particleSystem->Update(dt);

	scenario.Update(*this, ui, camera);
	camera.Update(dt);
	shaderData.Update(camera);
}

void Scene::Render()
{
	GSRainShader.Render(shaderData, *this);
	regularShader.Render(shaderData, *this);
}