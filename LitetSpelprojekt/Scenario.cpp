#include "Scenario.h"

#include <random>
#include <chrono>

Scenario::Scenario()
{
	Suspect testsuspect;
	testsuspect.age = 34;
	testsuspect.information.info = "I have some information about Another person";
	testsuspect.information.connections[0] = "Another person";
	testsuspect.name = "Steve";

	suspects.push_back(testsuspect);
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
		clues[i].model.get()->SetPosition(clueLocations[i]);
		clues[i].model.get()->Update(Graphics::GetDeviceContext());
	}
}

bool Scenario::TempLoadClues(std::string path)
{
	Clue clue;
	clue.model = std::make_shared<Model>();

	if (!clue.model->Initialize(Graphics::GetDevice(), path))
	{
		Error("FAILED TO INITIALIZE CLUE");
		return false;
	}

	clues.push_back(clue);

	return true;
}

void Scenario::Run(InGameUI& ui, Camera& camera)
{
	bool hoveringClue = false;

	std::string names[] = { "Test 1", "Test 2", "Test 3", "Test 4", };

	for (auto& clue : clues)
	{
		if (camera.CheckIntersection(clue.model->boundingbox))
		{
			hoveringClue = true;

			if (Event::GetCurrentEvent() == EventType::LEFTCLICK)
			{
				ui.journal.AddSuspect(identifiedSuspects, names[identifiedSuspects], false);
				identifiedSuspects++;

				clue.found = true;

				if (clue.murderClue == true)
					Print("I am a murder clue!");

				clue.model.get()->SetPosition({ 0, -100, 0 });
				clue.model.get()->Update(Graphics::GetDeviceContext());
				ui.ShowNotification();
			}
		}
	}

	if (hoveringClue)
		ui.SetCursorType(CursorType::CLUE);

	else
		ui.SetCursorType(CursorType::CROSS);
}