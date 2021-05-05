#include "Scene.h"

Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, 0.001f, 40.0f, { 0, 10, 30 })
{
	Importer::LoadScene("Models/World.mff");	
	Importer::Initialize(Graphics::GetDevice());

	std::vector<Mesh> meshes = Importer::Data::GetMeshes();
	for (auto& mesh : meshes)
	{
		auto model = std::make_shared<Model>(mesh);
		models.push_back(model);
	}

	sh.Initialize(window);
	AddLight();

	//scenario.InitializeClueLocations();

	//scenario.TempLoadClues("Models/testclue.obj");
	//scenario.TempLoadClues("Models/testclue2.obj");
	//scenario.TempLoadClues("Models/testclue3.obj");
	//scenario.TempLoadClues("Models/testclue4.obj");

	//for (auto& clue : scenario.clues)
	//{
	//	models.push_back(clue.model);
	//	gameObjects.push_back(clue.model);
	//}

	//scenario.SetRandomizedLocations();
}

bool Scene::AddModel(const std::string& path)
{
	auto model = std::make_shared<Model>();
	models.push_back(model);
	gameObjects.push_back(model);
	//if (!model->Initialize(Graphics::GetDevice(), path))
	//{
	//	Error("FAILED TO INITIALIZE MODEL");
	//	return false;
	//}
	return true;
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

	for (auto& model : models)
	{
		bool hit = false;

		switch (model->collidertype)
		{
		case ColliderType::BOX:
			hit = camera.CheckCollision(model->boundingbox);
			break;
		case ColliderType::SPHERE:
			break;
		}

		if (hit)
		{
			camera.SetPosition(lastPosition);
			XMFLOAT3 direction = { camera.GetPosition().x - model->GetPosition().x, 0.0f,
									camera.GetPosition().z - model->GetPosition().z };
			camera.PushBack(direction, dt);
		}
	}

	scenario.Run(ui, camera);
	camera.Update(dt);
}

void Scene::Render()
{
	sh.Render(*this);
}