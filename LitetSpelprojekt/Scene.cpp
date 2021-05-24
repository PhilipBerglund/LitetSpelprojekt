#include "Scene.h"

Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, 0.001f, 50.0f, { 0, 15, 0 })
{
	//Längst upp om man ska rita ut skitn (som alla andra modeller)
	//Importer::LoadScene("Models/Office.mff");
	//Importer::LoadScene("Models/Bar.mff");
	//Importer::LoadScene("Models/Hotel.mff");
	//Importer::LoadScene("Models/Restaurant.mff");
	//Importer::LoadScene("Models/Park.mff");
	//Importer::LoadScene("Models/Objects.mff");
	//Importer::LoadScene("Models/Houses.mff");
	//Importer::LoadScene("Models/Streets.mff");

	Importer::LoadScene("Models/NewTestTranslation.mff");
	Importer::Initialize(Graphics::GetDevice());

	for (int i = 0; i < Importer::Data::scenes.size(); ++i)
	{
		for (auto& mesh : Importer::Data::GetMeshes(i))
		{
			auto model = std::make_shared<Model>(mesh);
			models.insert(std::make_pair(model->GetName(), model));
		}
	}

	//bounds = Bounds("Models/BBoxes.mff");

	AddRainParticleSystem(3000, 150, 200);
	AddSmokeParticleSystem(200, 5, 10, { 25.0f, 10.0f, 40.0f, 1.0f }, 60);
	AddSmokeParticleSystem(400, 5, 10, { 0,0,0, 1.0f }, 200);
	AddLight();

	//scenario = Scenario(*this);
}


void Scene::AddRainParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity)
{
	auto particleSystem = std::make_shared<RainSystem>(maxParticles, minVelocity, maxVelocity);
	rainSystem.push_back(particleSystem);
}

void Scene::AddSmokeParticleSystem(UINT maxParticles, float minVelocity, float maxVelocity, XMFLOAT4 startPos, float maxParticleRange)
{
	auto particleSystem = std::make_shared<SmokeSystem>(maxParticles, minVelocity, maxVelocity, startPos, maxParticleRange);
	smokeSystem.push_back(particleSystem);
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
	camera.Update(dt);

	for (auto& box : bounds.boxes)
	{
		if (box.Intersects(camera.boundingsphere))
		{
			camera.SetPosition(lastPosition);
			/*XMVECTOR direction = { camera.GetPosition().x - box.Center.x, 0.0f,
									camera.GetPosition().z - box.Center.z };
			direction = XMVector3Normalize(direction);
			XMFLOAT3 dir;
			XMStoreFloat3(&dir, direction);
			camera.PushBack(dir, dt);*/
			break;
		}
	}

	for (auto& model : models)
		animator.PlayAnimation(dt, *model.second);

	for (auto& particleSystem : rainSystem)
		particleSystem->Update(dt);

	for (auto& particleSystem : smokeSystem)
		particleSystem->Update(dt);

	scenario.Update(*this, ui, camera);
	shaderData.Update(camera);
}

void Scene::Render()
{
	GSRainShader.RenderRain(shaderData, *this);
	GSSmokeShader.RenderSmoke(shaderData, *this);
	regularShader.Render(shaderData, *this);
}