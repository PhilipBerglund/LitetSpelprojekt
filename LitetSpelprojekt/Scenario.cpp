#include "Scenario.h"
#include "Scene.h"

#include <random>
#include <chrono>

Clue::Clue(std::string path, XMFLOAT3 position)
{
	Importer::LoadScene(path);
	information = "Description of item and where it was found";

	Mesh mesh = Importer::Data::GetMeshAt(Importer::Data::scenes.size() - 1, 0);
	model = std::make_shared<Model>(mesh);
	model->SetPosition(position);
	model->Update(Graphics::GetDeviceContext());
}

Suspect::Suspect(std::string path, XMFLOAT3 position)
{
	Importer::LoadScene(path);
	Mesh mesh = Importer::Data::GetMeshAt(Importer::Data::scenes.size() - 1, 0);
	model = std::make_shared<Model>(mesh);
	model->SetScale({2.5f,2.5f,2.5f});
	model->SetPosition(position);
	model->Update(Graphics::GetDeviceContext());
	model->boundingbox.Center.y += 10;
}

Scenario::Scenario(Scene& scene)
{
	Suspect testSuspect("Models/TestSuspect.mff", {50, 0, -30});
	testSuspect.name = "A";
	testSuspect.age = 55;
	testSuspect.height = 180;
	testSuspect.shoeSize = 45;
	testSuspect.information.info = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
	testSuspect.information.connections[0] = "B";
	testSuspect.information.numConnections = 1;
	testSuspect.information.rumours[0] = "According to A, B looked sketchy last night";
	testSuspect.information.valueable = true;
	testSuspect.characteristics[0] = "Likes cats.";
	testSuspect.characteristics[1] = "monke";
	testSuspect.characteristics[2] = "mmmmmmmm, monke";

	scene.AddModel(testSuspect.model);
	suspects.push_back(testSuspect);

	Suspect testSuspect2("Models/TestSuspect.mff", { 50, 0, -20 });
	testSuspect2.name = "B";
	testSuspect2.age = 28;
	testSuspect2.height = 167;
	testSuspect2.shoeSize = 38;
	testSuspect2.information.info = ". . .";
	testSuspect2.information.valueable = false;
	testSuspect2.characteristics[0] = "Does not like A.";
	testSuspect2.characteristics[1] = "monkeeeeh";
	testSuspect2.characteristics[2] = "mmmmmmmmhHHHHHHHHH, monke XDDDDDDD";

	scene.AddModel(testSuspect2.model);
	suspects.push_back(testSuspect2);

	Clue clue("Models/TestClue.mff", {-20, 10, 40});
	scene.AddModel(clue.model);
	clues.push_back(clue);

	Clue clue2("Models/TestClue.mff", { -50, 10, -40 });
	scene.AddModel(clue2.model);
	clues.push_back(clue2);

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

//bool Scenario::TempLoadClues(std::string path)
//{
//	Clue clue("");
//	clue.model = std::make_shared<Model>();
//
//	//if (!clue.model->Initialize(Graphics::GetDevice(), path))
//	//{
//	//	Error("FAILED TO INITIALIZE CLUE");
//	//	return false;
//	//}
//
//	clues.push_back(clue);
//
//	return true;
//}

void Scenario::Update(Scene& scene, InGameUI& ui, Camera& camera)
{
	CursorType cursor = CursorType::CROSS;

	//CLUES
	bool hoveringClue = false;
	for (auto& clue : clues)
	{
		if (!clue.isFound && camera.CheckIntersection(clue.model->boundingbox))
		{
			hoveringClue = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				scene.models.erase(clue.model->GetName());
				clue.isFound = true;
			}
		}
	}

	if (hoveringClue)
		cursor = CursorType::CLUE;

	//SUSPECTS
	static std::string lastSuspect;
	bool chattingSuspect = false;
	bool hoveringSuspect = false;
	for (auto& suspect : suspects)
	{
		if (suspect.name == lastSuspect)
			return;

		if (camera.CheckIntersection(suspect.model->boundingbox))
		{
			hoveringSuspect = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				ui.chatOverlay.SetUp(suspect.name, suspect.information.info);

				if (!ui.journal.HasSuspect(suspect.name))
				{
					ui.journal.AddSuspect(identifiedSuspects, suspect.name, suspect.characteristics[0], suspect.characteristics[1], suspect.characteristics[2], suspect.age, suspect.height, suspect.shoeSize);
					suspect.fullyKnown = true;
					identifiedSuspects++;

					if (suspect.information.numConnections > 0)
					{
						for (int i = 0; i < suspect.information.numConnections; ++i)
						{
							if (!ui.journal.HasSuspect(suspect.information.connections[i]))
								ui.journal.AddSuspect(identifiedSuspects, suspect.information.connections[i]);

							ui.journal.AddRumour(suspect.information.connections[i], suspect.information.rumours[i]);
						}
					}

					ui.ShowNotification();
				}

				else if (ui.journal.HasSuspect(suspect.name) && !suspect.fullyKnown)
				{
					ui.journal.UpdateSuspect(suspect.name, suspect.characteristics[0], suspect.characteristics[1], suspect.characteristics[2], suspect.age, suspect.height, suspect.shoeSize);
					suspect.fullyKnown = true;
					ui.ShowNotification();
				}

				chattingSuspect = true;
				GameSettings::SetState(GameState::CHAT);
				lastSuspect = suspect.name;
			}
		}
	}

	if (hoveringSuspect)
		cursor = CursorType::CHAT;

	else if (chattingSuspect)
		cursor = CursorType::NONE;

	ui.SetCursorType(cursor);
}