#include "Scene.h"
#include "SoundHandler.h"


Scene::Scene( UINT windowWidth, UINT windowHeight, HWND window)
	:camera(XM_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f, 0.0015f, 50.0f, { 0, 17, 0 })
{
	Importer::LoadScene("Models/Office.mff");
	Importer::LoadScene("Models/Bar.mff");
	Importer::LoadScene("Models/Hotel.mff");
	Importer::LoadScene("Models/Restaurant.mff");
	Importer::LoadScene("Models/Park.mff");
	Importer::LoadScene("Models/OutsideObjects.mff");
	Importer::LoadScene("Models/Houses.mff");

	Importer::Initialize(Graphics::GetDevice());

	for (int i = 0; i < Importer::Data::scenes.size(); ++i)
	{
		for (auto& mesh : Importer::Data::GetMeshes(i))
		{
			auto model = std::make_shared<Model>(mesh);
			models.insert(std::make_pair(model->GetName(), model));
		}
	}
    
	Importer::LoadScene("Models/Streets.mff");
	for (auto& noShadowMesh : Importer::Data::GetMeshes(Importer::Data::scenes.size()-1))
	{
		auto noShadowModel = std::make_shared<Model>(noShadowMesh);
		nonShadowModels.insert(std::make_pair(noShadowModel->GetName(), noShadowModel));
	}
	
	Importer::Initialize(Graphics::GetDevice());
	scenario = Scenario(*this);

	//QuadTree Setup
	QTSquare QTbounds;
	QTbounds.h = 600;
	QTbounds.w = 600;
	QTbounds.xPos = 0;
	QTbounds.zPos = -45;
	SetupQuadTree(this->tree, QTbounds, 200);
	for (auto& mod : models)
	{
		this->tree->InsertModel(mod.second);
	}
	this->frust.Update(this->camera);
   
	bounds = Bounds("Models/BBoxes.mff");

	AddRainParticleSystem(3000, 150, 200);
	AddSmokeParticleSystem(200, 5, 10, { 23.5f, 5.0f, -40.0f, 1.0f }, 60);
	AddSmokeParticleSystem(400, 5, 10, { -110.0f, 110.0f, -8.5f, 1.0f }, 200);
	AddLight();
	lights[0]->SetRotation({ 0.0f,-10.0f,10.0f });
	AddShadowMap(Window::GetWidth(), Window::GetHeight());
	soundHandler.AddAudio(L"Audio/JessicaWoolfBackgroundMusic.wav");
	soundHandler.SetVolume(0.3f);
	soundHandler.StartAudio();

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
	//nonShadowModels.insert(std::make_pair(model->GetName(), model));
}

void Scene::AddLight()
{
	auto light = std::make_shared<Light>();
	lights.push_back(light);
	gameObjects.push_back(light);
}

void Scene::AddShadowMap(float width, float height)
{
	auto shadowMap = std::make_shared<ShadowMap>(width, height);
	shadowMaps.push_back(shadowMap);
}

void Scene::Reset(InGameUI& ui)
{
	ui.Reset();
	scenario.Reset();
}

void Scene::Update(InGameUI& ui, float dt)
{
	this->ClearQTModels();
	XMFLOAT3 lastPosition = camera.GetPosition();
	camera.Update(dt);
	this->frust.Update(this->camera);
	QTIntersect(this->frust, this->tree, this->QTModels);

	for (auto& box : bounds.boxes)
	{
		if (box.Intersects(camera.boundingsphere))
		{
			camera.SetPosition(lastPosition);
			break;
		}
	}

	for (auto& particleSystem : rainSystem)
		particleSystem->Update(dt);

	for (auto& particleSystem : smokeSystem)
		particleSystem->Update(dt);

	scenario.Update(*this, ui, camera);
  
	shaderData.Update(camera, *lights[0]);
	//Print("NrOfModels: " + std::to_string(this->QTModels.size()));
}

void Scene::Render()
{
	RenderShadowMap();
	GSRainShader.RenderRain(shaderData, *this);
	GSSmokeShader.RenderSmoke(shaderData, *this);
	regularShader.Render(shaderData, *this);
}

void Scene::RenderShadowMap()
{
	ShadowMapShader.RenderShadowMap(shaderData, *this);
	Graphics::BeginFrame();
}