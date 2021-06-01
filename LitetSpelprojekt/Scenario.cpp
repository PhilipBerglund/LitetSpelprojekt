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
	// --------- CLUES ----------

	// --- Chalk Outline --- 
	Clue chalkOutline("Models/ChalkOutLine.mff", { -110, 1, -90});
	chalkOutline.information = "This is where the Hotel Director Lloyd was murdered last night...\n  I should ask around.";
	chalkOutline.model->SetScale({ 1.2, 1.2f,1.2f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	chalkOutline.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(chalkOutline.model);
	clues.push_back(chalkOutline);

	// --- Shoe Print(s) --- 
	Clue leftShoePrint("Models/ShoePrints.mff", { -135.0f, 0.0f, -135.0f});
	leftShoePrint.information = "A trail of shoeprints. I wonder...";
	leftShoePrint.model->SetScale({ 0.9f, 0.9f, 0.9f });
	leftShoePrint.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(leftShoePrint.model);
	clues.push_back(leftShoePrint);

	// --- Hair -- 
	Clue hair("Models/HairBlack.mff", { -129.0f, 1.0f, -66.0f });
	hair.information = "What is this? A cluster of black hair?";
	scene.AddModel(hair.model);
	clues.push_back(hair);

	// --- Revolver ---
	Clue revolver("Models/Revolver.mff", { 100.0f, 5.0f, -186.0f });
	revolver.information = "A revolver!? Who in their right mind would toss a weapon in the garbage bin?";
	scene.AddModel(revolver.model);
	clues.push_back(revolver);

	// --- Revolver shells in the bar
	Clue revolverShells("Models/ShellBox.mff", { -162.0f, 5.0f, -32.0f });
	revolverShells.information = "This looks like revolver shells, and some are missing from the box.";
	revolverShells.model->SetScale({ 0.3f, 0.3f, 0.3f });
	revolverShells.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(revolverShells.model);
	clues.push_back(revolverShells);

	// --- Anonymous notes --- 

		// -- Bar -- //
	
		// -- Park -- //

		// -- Hotel -- //
	Clue noteNoRegrets("Models/NoteNoRegrets.mff", { -116.0f, 1.0f, -103.0f });
	noteNoRegrets.information = "Is this a note from the murderer? It reads: 'I don't regret what I did! Lloyd had it coming.'";
	noteNoRegrets.model->SetScale({ 0.5f,0.5f,0.5f });
	noteNoRegrets.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(noteNoRegrets.model);
	clues.push_back(noteNoRegrets);

		// -- Resturaunt -- // -- Fixa texten Sophia
	Clue newsPaper("Models/NewsPaper.mff", { 223.0f, 8.7f, 75.0f });
	newsPaper.information = "The newspapers headline says; 'Hotel Owner buys 'The Golden Rooster''";
	newsPaper.model->SetScale({ 0.2f,0.2f,0.2f });
	newsPaper.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(newsPaper.model);
	clues.push_back(newsPaper);

		// -- Market -- By Mr-Wicker//
	Clue noteNeedHelp("Models/NoteNeedHelp.mff", { -97.2f, 1.0f, 109.0f, });
	noteNeedHelp.information = "'I need your help, you are the only one I can talk to - a friend'";
	noteNeedHelp.model->SetScale({ 0.5f,0.5f,0.5f });
	noteNeedHelp.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(noteNeedHelp.model);
	clues.push_back(noteNeedHelp);

		// -- Office -- //
	Clue prompt("Models/PromptPoster.mff", { 111.0f, 22.0f, 110.0f });
	prompt.information = "A note saying they need me to solve the murder.";
	prompt.model->SetScale({ 2.0f, 2.0f, 2.0f });
	prompt.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(prompt.model);
	clues.push_back(prompt);

	// --------- NPCs ----------

	// --- Inside the bar ---

	// --- Fabian Voltaire (bartender) ---
	Suspect bartender("Models/Bartender1.mff", { -172, 16, -22 });
	bartender.name = "Fabian Voltaire";
	bartender.age = 34;
	bartender.height = 160;
	bartender.shoeSize = 45;
	bartender.information.info = "You're asking me if I saw something last night? I didn't see anything I'll tell you, but Franklin over there by the pool table, he's the towns know-it-all. He might know something...";
	bartender.information.connections[0] = "Franklin Pickett";
	bartender.information.numConnections = 1;
	bartender.information.rumours[0] = "The towns know-it-all. - Fabian";
	bartender.information.valueable = true;
	bartender.characteristics[0] = "Blonde hair";
	bartender.characteristics[1] = "Unlikeabale";
	bartender.characteristics[2] = "Large feet";
	bartender.model->SetScale({ 1.35f, 1.35f, 1.35f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	bartender.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(bartender.model);
	suspects.push_back(bartender);

	// --- Franklin Pickett (know-it-all) ---
	Suspect knowItAll("Models/KnowItAll.mff", { -196, 14, 54});
	knowItAll.name = "Franklin Pickett";
	knowItAll.age = 50;
	knowItAll.height = 173;
	knowItAll.shoeSize = 39;
	knowItAll.information.info = "They say I know things. Who murdered Mayor Rock is one of the few things I don't know, but one thing I know for sure is that I'm the only one who comes here, if you know what I mean.";
	knowItAll.information.connections[0] = "Fabian Voltaire";
	knowItAll.information.numConnections = 1;
	knowItAll.information.rumours[0] = "Isn't liked by the locals. - Franklin";
	knowItAll.information.valueable = true;
	knowItAll.characteristics[0] = "Black hair";
	knowItAll.characteristics[1] = "Not very talkative";
	knowItAll.characteristics[2] = "Tiny shoes";
	knowItAll.model->SetScale({ 1.35f, 1.35f, 1.35f });
	//chalkOutline.model->SetRotation({ 0.0f, 0, 0.0f });
	knowItAll.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(knowItAll.model);
	suspects.push_back(knowItAll);


	// --- Inside the Hotel --- 
	
	// --- Receptionist (Sally Bennett) ---
	//Suspect receptionist("Models/Receptionist.mff", { -116.0f, 0, -120.0f });
	Suspect receptionist("Models/Receptionist.mff", { -115.0f, 15, -125.0f});

	receptionist.name = "Sally Bennett";
	receptionist.age = 28;
	receptionist.height = 178;
	receptionist.shoeSize = 38;
	receptionist.information.info = "Poor Lloyd, he loved this town. Looks like I'll be the one running this hotel from now. For the record, I was out with Mr. Wicker last night.";
	receptionist.information.connections[0] = "Mr. Wicker";
	receptionist.information.numConnections = 1;
	receptionist.information.rumours[0] = "Sally said they were together last night.";
	receptionist.information.valueable = true;
	receptionist.characteristics[0] = "Brown hair";
	receptionist.characteristics[1] = "Likeable";
	receptionist.characteristics[2] = "Small feet";
	receptionist.model->SetScale({ 2.0f, 2.0f, 2.0f});
	receptionist.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(receptionist.model);
	suspects.push_back(receptionist);


	// --- At the park --- 

	// --- Mayor Rock --- 
	Suspect mayor("Models/Mayor.mff", { 144, 13.75, -108.5f });
	mayor.name = "Mayor A. Rock";
	mayor.age = 62;
	mayor.height = 171;
	mayor.shoeSize = 44;
	mayor.information.info = "That bastard Lloyd had it coming. Sooner or later someone would have killed him. The new girl, Claudette, keeps staring at me from the restaurant. Should I be concerned?";
	mayor.information.connections[0] = "Claudette Tasse";
	mayor.information.numConnections = 1;
	mayor.information.rumours[0] = "Watches the Mayor quite alot. - Mayor";
	mayor.information.valueable = true;
	mayor.characteristics[0] = "Gray hair";
	mayor.characteristics[1] = "Large";
	mayor.characteristics[2] = "Rich";
	mayor.model->SetScale({ 1.5f, 1.5f, 1.5f });

	mayor.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(mayor.model);
	suspects.push_back(mayor);

	// --- Timothy Cooke --- 
	Suspect boy("Models/Boy.mff", { 38, 18, -180 });
	boy.name = "Timothy Cooke";
	boy.age = 18;
	boy.height = 181;
	boy.shoeSize = 42;
	boy.information.info = "That Sally girl sure is thrifty that she worked at the hotel when all of this happened... I'm a bit jealous of all that sweet money she's gonna get from running that lousy hotel in his place.";
	boy.information.connections[0] = "Sally Bennett";
	boy.information.numConnections = 1;
	boy.information.rumours[0] = "Benefits from the death of Lloyd. - Timothy";
	boy.information.valueable = true;
	boy.characteristics[0] = "Smoker";
	boy.characteristics[1] = "Jealous";
	boy.characteristics[2] = "Cocky";
	boy.model->SetScale({ 1.5f, 1.5f, 1.5f });

	boy.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(boy.model);
	suspects.push_back(boy);

	/// --- At the market --- 

	// --- John Wick --- 
	Suspect marketMan("Models/StandingMan.mff", { -95.0f, 17.5f, 152.0f });
	marketMan.name = "Mr. Wicker";
	marketMan.age = 36;
	marketMan.height = 185;
	marketMan.shoeSize = 45;
	marketMan.information.info = "Business is not doing so well, since you are here, want to buy something? If you're wondering where I was last night, I was at the shooting range like I am every night. Usually with Voltaire, but he never showed up.";
	marketMan.information.connections[0] = "Fabian Voltaire";
	marketMan.information.numConnections = 1;
	marketMan.information.rumours[0] = "Fabian is Mr. Wicker's favorite customer";
	marketMan.information.valueable = true;
	marketMan.characteristics[0] = "Mustache";
	marketMan.characteristics[1] = "Large feet";
	marketMan.characteristics[2] = "Likes guns";
	marketMan.model->SetScale({ 1.3f, 1.3f, 1.3f });
	marketMan.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(marketMan.model);
	suspects.push_back(marketMan);

	// --- Old Lady ---
	Suspect lady("Models/OldLady.mff", { 5.0f, 14.0f, 148.0f});
	lady.name = "Olga Morozov";
	lady.age = 79;
	lady.height = 153;
	lady.shoeSize = 37;
	lady.information.info = "Oh... This is so awful. I hear rumors about it being Sally but it cannot be her! She's so truthful and kind, she would never hurt a fly. The grumpy bartender on the other hand...";
	lady.information.connections[0] = "Sally Bennett";
	lady.information.connections[1] = "Fabian Voltaire";
	lady.information.numConnections = 2;
	lady.information.rumours[0] = "Most honest person in town. - Olga";
	lady.information.rumours[1] = "Grumpy -- Olga";
	lady.information.valueable = true;
	lady.characteristics[0] = "Old and slow";
	lady.characteristics[1] = "Collects matryoshka dolls";
	lady.characteristics[2] = "The town grandma";
	lady.model->SetScale({ 1.4f, 1.4f, 1.4f });
	lady.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(lady.model);
	suspects.push_back(lady);

	// --- Civilians ---
	
	// --- Claudette Tasse ---
	Suspect claudette("Models/FemaleCivilian.mff", { 213.0f, 15.7, 54.0f });
	claudette.name = "Claudette Tasse";
	claudette.age = 23;
	claudette.height = 165;
	claudette.shoeSize = 36;
	claudette.information.info = "It's been two weeks since I moved here and there has already been a murder... I hope this was a one time occurrence. See Mayor Rock over there? He looks awfully happy considering someone was murdered in his town last night...";
	claudette.information.connections[0] = "Mayor A. Rock";
	claudette.information.numConnections = 1;
	claudette.information.rumours[0] = "The Mayor seems awfully happy. - Claudette";
	claudette.information.valueable = true;
	claudette.characteristics[0] = "Brown hair";
	claudette.characteristics[1] = "Small";
	claudette.characteristics[2] = "French";
	claudette.model->SetScale({ 1.5f, 1.5f, 1.5f });

	claudette.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(claudette.model);
	suspects.push_back(claudette);

	// --- At the Restaurant --- 
	 
	// --- Tess Singer ---
	Suspect singer("Models/Burlesque.mff", { 139, 20, 156 });
	singer.name = "Tess Singer";
	singer.age = 31;
	singer.height = 170;
	singer.shoeSize = 38;
	singer.information.info = "I was looking forward to perform at the bigger and better 'Private Lion', now I'm stuck performing here... I guess the drunk at 'The Golden Rooster' is happy.";
	singer.information.connections[0] = "Franklin Pickett";
	singer.information.connections[1] = "Fabian Voltaire";
	singer.information.numConnections = 2;
	singer.information.rumours[0] = "Happy that the bar stays. - Tess";
	singer.information.rumours[1] = "Can keep his bar because of the murder. - Tess";
	singer.information.valueable = true;
	singer.characteristics[0] = "Red hair";
	singer.characteristics[1] = "Energetic";
	singer.characteristics[2] = "Loves to perform at big theaters";
	singer.model->SetScale({ 1.5f, 1.5f, 1.5f });
	singer.model->Update(Graphics::GetDeviceContext());
	scene.AddModel(singer.model);
	suspects.push_back(singer);



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
							{
								ui.journal.AddSuspect(identifiedSuspects, suspect.information.connections[i]);
								identifiedSuspects++;
							}
								
							ui.journal.AddRumour(suspect.information.connections[i], suspect.information.rumours[i]);
							ui.ShowNotification();
						}
					}

					ui.ShowNotification();
				}

				else if (ui.journal.HasSuspect(suspect.name) && !suspect.fullyKnown)
				{
					ui.journal.UpdateSuspect(suspect.name, suspect.characteristics[0], suspect.characteristics[1], suspect.characteristics[2], suspect.age, suspect.height, suspect.shoeSize);
					suspect.fullyKnown = true;

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
			}
		}
	}

	if (hoveringSuspect)
		cursor = CursorType::CHAT;

	ui.SetCursorType(cursor);
}