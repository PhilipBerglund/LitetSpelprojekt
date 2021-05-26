#include "Scenario.h"
#include "Scene.h"

#include <random>
#include <chrono>

Clue::Clue(std::string path, XMFLOAT3 position)
{
	Importer::LoadScene(path);
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
	model->boundingbox.Center.y += 15;
}

Scenario::Scenario(Scene& scene)
{
	//Suspect testSuspect("Models/TestSuspect.mff", {50, 0, -30});
	//testSuspect.name = "A";
	//testSuspect.age = 55;
	//testSuspect.height = 180;
	//testSuspect.shoeSize = 45;
	//testSuspect.information.info = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
	//testSuspect.information.connections[0] = "B";
	//testSuspect.information.numConnections = 1;
	//testSuspect.information.rumours[0] = "According to A, B looked sketchy last night";
	//testSuspect.information.valueable = true;
	//testSuspect.characteristics[0] = "Likes cats.";
	//testSuspect.characteristics[1] = "monke";
	//testSuspect.characteristics[2] = "mmmmmmmm, monke";
	//scene.AddModel(testSuspect.model);
	//suspects.push_back(testSuspect);

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


	// --------- CLUES ----------

	// --- Chalk Outline --- 
	Clue chalkOutline("Models/ChalkOutLine.mff", { -125, 1, -90});
	chalkOutline.information = "This is where Mayor Rock was murdered last night...\n  I should look around.";
	chalkOutline.model->SetScale({ 1.2, 1.2f,1.2f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	chalkOutline.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(chalkOutline.model);
	clues.push_back(chalkOutline);

	// --- Shoe Print(s) --- 
	Clue leftShoePrint("Models/ShoePrints.mff", { -135.0f, 0.0f, -135.0f});
	leftShoePrint.information = "A trail of shoeprints. I wonder...";

	scene.AddModel(leftShoePrint.model);
	clues.push_back(leftShoePrint);

	// --- Hair -- 
	Clue hair("Models/HairBlack.mff", { -129.0f, 1.0f, -66.0f });
	hair.information = "What is this? A cluster of black hair?";
	scene.AddModel(hair.model);
	clues.push_back(hair);

	// --------- NPCs ----------

	// --- Inside the bar ---

	// --- Fabian Voltaire (bartender) ---
	Suspect bartender("Models/Bartender.mff", { -172, 17, -22 });
	bartender.name = "Fabian Voltaire";
	bartender.age = 34;
	bartender.height = 180;
	bartender.shoeSize = 45;
	bartender.information.info = "You're asking me if I saw something last night? I didn't see anything I'll tell you, but Franklin over there by the pool table, he's the towns know-it-all. He might know something...";
	bartender.information.connections[0] = "Franklin Pickett";
	bartender.information.numConnections = 1;
	bartender.information.rumours[0] = "The towns know-it-all.";
	bartender.information.valueable = false;
	bartender.characteristics[0] = "Loves gin and tonic.";
	bartender.characteristics[1] = "Tries hard to be liked";
//	bartender.characteristics[2] = "mmmmmmmm, monke";
	bartender.model->SetScale({ 1.35f, 1.35f, 1.35f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	bartender.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(bartender.model);
	suspects.push_back(bartender);

	// --- Franklin Pickett (know-it-all) ---
	Suspect knowItAll("Models/KnowItAll.mff", { -213, 17, 54 });
	knowItAll.name = "Franklin Pickett";
	knowItAll.age = 50;
	knowItAll.height = 173;
	knowItAll.shoeSize = 39;
	knowItAll.information.info = "";
	knowItAll.information.connections[0] = "Fabian Voltaire";
	knowItAll.information.numConnections = 1;
	knowItAll.information.rumours[0] = "According to Franklin, Voltaire isn't all that liked by the towns inhabitants.";
	knowItAll.information.valueable = true;
	knowItAll.characteristics[0] = "Black Hair";
	knowItAll.characteristics[1] = "Not very talkative";
	knowItAll.characteristics[2] = "Tiny shoes";
	knowItAll.model->SetScale({ 1.35f, 1.35f, 1.35f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	knowItAll.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(knowItAll.model);
	suspects.push_back(knowItAll);

	//Clue clue("Models/TestClue.mff", {-20, 10, 40});
	//clue.information = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
	//scene.AddModel(clue.model);
	//clues.push_back(clue);

	//Clue clue2("Models/TestClue.mff", { -50, 10, -40 });
	//clue2.information = "Ooga booga.";
	//scene.AddModel(clue2.model);
	//clues.push_back(clue2);

	Importer::Initialize(Graphics::GetDevice());
}

void Scenario::Reset()
{
	identifiedSuspects = 0;
	foundClues = 0;

	for (auto& clue : clues)
		clue.isFound = false;

	for (auto& suspect : suspects)
		suspect.fullyKnown = false;
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

void Scenario::Update(Scene& scene, InGameUI& ui, Camera& camera)
{
	static std::string lastSuspect;

	//IF CONVICED
	if (ui.Convict())
	{
		if (lastSuspect != murderer)
			GameSettings::SetState(GameState::END_LOSS);
		else
			GameSettings::SetState(GameState::END_WIN);

		Event::DispatchEvent(EventType::STATECHANGE);
	}

	if (GameSettings::GetState() != GameState::INGAME)
		return;

	CursorType cursor = CursorType::CROSS;

	//CLUES
	bool hoveringClue = false;
	for (auto& clue : clues)
	{
		if (camera.CheckIntersection(clue.model->boundingbox))
		{
			hoveringClue = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				if (!clue.isFound)
				{
					foundClues++;
					clue.isFound = true;
					clue.ID = foundClues;
				}

				ui.clueOverlay.SetUp(clue.ID, clue.information);
				hoveringClue = false;
				cursor = CursorType::NONE;
			}
		}
	}

	if (hoveringClue)
		cursor = CursorType::CLUE;

	//SUSPECTS
	bool hoveringSuspect = false;
	for (auto& suspect : suspects)
	{
		if (camera.CheckIntersection(suspect.model->boundingbox))
		{
			hoveringSuspect = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				lastSuspect = suspect.name;

				if (suspect.fullyKnown)
					ui.chatOverlay.SetUp(suspect.name, suspect.information.info);
				else
					ui.chatOverlay.SetUp(suspect.name, suspect.information.info, true);

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
			}
		}
	}

	if (hoveringSuspect)
		cursor = CursorType::CHAT;

	ui.SetCursorType(cursor);
}