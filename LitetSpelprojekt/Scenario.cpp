#include "Scenario.h"
#include <algorithm>
#include <random>
#include <chrono>


void Scenario::InitializeClueLocations()
{
	clueLocations.push_back({ 5,0,0 });
	clueLocations.push_back({ 7,0,0 });
	clueLocations.push_back({ 2,0,9 });
	clueLocations.push_back({ 9,0,2 });

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

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

void Scenario::Run(InGameUI& ui, InputHandler& input, float dt, Camera& camera)
{
	bool isHoveringClue = false;

	for (auto& clue : clues)
	{
		//bool hit = false;

		//switch (clue.model->collidertype)
		//{
		//case ColliderType::BOX:
		//	hit = camera.CheckCollision(clue.model->boundingbox);
		//	break;
		//case ColliderType::SPHERE:
		//	break;
		//}

		if (camera.CheckIntersection(clue.model->boundingbox))
		{
			isHoveringClue = true;

			if (input.LeftIsClicked())
			{
				clue.found = true;

				Print("Clue clicked!");

				if (clue.murderClue == true)
					Print("I am a murder clue!");

				clue.model.get()->SetPosition({ 0, -100, 0 });
				clue.model.get()->Update(Graphics::GetDeviceContext());
				ui.ShowNotification();
				//notificationTime = 5;
			}
		}
	}

	if (isHoveringClue)
		ui.SetCursorType(CursorType::CLUE);

	else
		ui.SetCursorType(CursorType::CROSS);

	//if (notificationTime > 0 && notificationTime < 100)
	//{
	//	notificationTime -= dt;
	//}

	//if (notificationTime <= 0)
	//{
	//	ui.ShowNotification();
	//	notificationTime = 101.0f;
	//}
}