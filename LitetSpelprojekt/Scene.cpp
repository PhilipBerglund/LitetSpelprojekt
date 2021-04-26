#include "Scene.h"

Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f, { 0,0,-10 })
{
	sh.Initialize(window);
	AddModel("Models/Troll.obj");
	AddLight();

	scenario.InitializeClueLocations();

	scenario.TempLoadClues("Models/testclue.obj");
	scenario.TempLoadClues("Models/testclue2.obj");
	scenario.TempLoadClues("Models/testclue3.obj");
	scenario.TempLoadClues("Models/testclue4.obj");

	for (auto& clue : scenario.clues)
	{
		models.push_back(clue.model);
		gameObjects.push_back(clue.model);
	}

	scenario.SetRandomizedLocations();
}

bool Scene::AddModel(const std::string& path)
{
	auto model = std::make_shared<Model>();
	models.push_back(model);
	gameObjects.push_back(model);
	if (!model->Initialize(Graphics::GetDevice(), path))
	{
		Error("FAILED TO INITIALIZE MODEL");
		return false;
	}
	return true;
}

void Scene::AddLight()
{
	auto light = std::make_shared<Light>();
	lights.push_back(light);
	gameObjects.push_back(light);
}

void Scene::Update(InGameUI& ui, InputHandler& input, float dt)
{
	camera.Rotate((float)input.ReadRawDelta().value().x, (float)input.ReadRawDelta().value().y);

	XMFLOAT3 lastPosition = camera.GetPosition();

	if (input.KeyIsPressed('W'))
		camera.MoveForward(dt);

	if (input.KeyIsPressed('S'))
		camera.MoveForward(-dt);

	if (input.KeyIsPressed('D'))
		camera.MoveRight(dt);

	if (input.KeyIsPressed('A'))
		camera.MoveRight(-dt);

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

		if (camera.CheckIntersection(model->boundingbox))
			Print("INTERSECTS");
	}

	scenario.Run(ui, input, dt, camera);

	camera.Update();
}

void Scene::Render()
{
	sh.Render(*this);
}