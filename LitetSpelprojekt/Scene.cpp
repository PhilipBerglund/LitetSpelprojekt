#include "Scene.h"

Scene::Scene(Graphics& graphics, UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f, { 0,0,-10 })
{
	sh.Initialize(graphics.GetDevice(), window);
	AddModel(graphics, "Models/Troll.obj");
	AddLight();

	scenario.TempLoadClues(graphics, "Models/gubbe.obj");

	for (auto& clue : scenario.clues)
	{
		models.push_back(clue.model);
		gameObjects.push_back(clue.model);
	}

	scenario.clues[0].model.get()->SetPosition({ 5,-5,0 });
	scenario.clues[0].model.get()->Update(graphics.GetDeviceContext());
}

bool Scene::AddModel(Graphics& graphics, const std::string& path)
{
	auto model = std::make_shared<Model>();
	models.push_back(model);
	gameObjects.push_back(model);
	if (!model->Initialize(graphics.GetDevice(), path))
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

void Scene::Update(InputHandler& input, float dt)
{
	camera.Rotate((float)input.ReadRawDelta().value().x, (float)input.ReadRawDelta().value().y);

	XMFLOAT3 lastPosition = camera.GetPosition();

	for (int i = 0; i < models.size(); ++i)

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

		Print(std::to_string(camera.CheckIntersection(model->boundingbox)));
	}

	camera.Update();
	scenario.Run(input, dt, camera);

}

void Scene::Render(Graphics& graphics)
{
	sh.Render(graphics.GetDeviceContext(), *this);
}