#include "Scenario.h"
#include "Scene.h"

#include <random>
#include <chrono>

Clue::Clue(std::string path)
{
	Importer::LoadScene(path);
	information = "I GOT SOME INFORMATION";

	Mesh mesh = Importer::Data::GetMeshes(Importer::Data::scenes.size() - 1).front();
	model = std::make_shared<Model>(mesh);
	model->SetPosition({ -30,10,90 });
	model->Update(Graphics::GetDeviceContext());
}

Scenario::Scenario(Scene& scene)
{
	Clue clue("Models/TestClue.mff");
	scene.AddModel(clue.model);
	clues.push_back(clue);
	Importer::Initialize(Graphics::GetDevice());
}

void Scenario::InitializeClueLocations()
{
	clueLocations.push_back({ 5,0,0 });
	clueLocations.push_back({ 7,0,0 });
	clueLocations.push_back({ 2,0,9 });
	clueLocations.push_back({ 9,0,2 });

	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(clueLocations.begin(), clueLocations.end(), std::default_random_engine(seed));
}

void Scenario::SetRandomizedLocations()
{
	for (int i = 0; i < clues.size(); i++)
	{
		clues[i].model->SetPosition(clueLocations[i]);
		clues[i].model->Update(Graphics::GetDeviceContext());
	}
}

bool Scenario::TempLoadClues(std::string path)
{
	Clue clue("");
	clue.model = std::make_shared<Model>();

	//if (!clue.model->Initialize(Graphics::GetDevice(), path))
	//{
	//	Error("FAILED TO INITIALIZE CLUE");
	//	return false;
	//}

	clues.push_back(clue);

	return true;
}

void Scenario::Update(Scene& scene, InGameUI& ui, Camera& camera)
{
	bool hoveringClue = false;

	std::string names[] = { "Test 1", "Test 2", "Test 3", "Test 4", };

	for (auto& clue : clues)
	{
		if (!clue.found && camera.CheckIntersection(clue.model->boundingbox))
		{
			hoveringClue = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				scene.models.erase(clue.model->GetName());
				clue.found = true;
				ui.ShowNotification();
			}
		}
	}

	if (hoveringClue)
		ui.SetCursorType(CursorType::CLUE);

	else
		ui.SetCursorType(CursorType::CROSS);
}