#include "Scenario.h"


bool Scenario::TempLoadClues(Graphics& graphics, std::string path)
{
	Clue clue;
	clue.model = std::make_shared<Model>();
	//auto model = std::make_shared<Model>();
	if (!clue.model->Initialize(graphics.GetDevice(), path))
	{
		Error("FAILED TO INITIALIZE CLUE");
		return false;
	}
	clues.push_back(clue);

	return true;
}

void Scenario::Run(Graphics& graphics, InGameUI ui, InputHandler& input, float dt, Camera& camera)
{
	for (auto& clue : clues)
	{
		bool hit = false;

		switch (clue.model->collidertype)
		{
		case ColliderType::BOX:
			hit = camera.CheckCollision(clue.model->boundingbox);
			break;
		case ColliderType::SPHERE:
			break;
		}


		if (camera.CheckIntersection(clue.model->boundingbox) && input.LeftIsClicked() == true)
		{
			Print("Clue clicked!");
			clue.model.get()->SetPosition({ 0, -100, 0 });
			clue.model.get()->Update(graphics.GetDeviceContext());
			ui.ShowNotification(true);
			notificationTime = 5;
		}
	}

	if (notificationTime > 0 && notificationTime < 100)
	{
		notificationTime -= dt;
	}

	if (notificationTime <= 0)
	{
		ui.ShowNotification(false);
		notificationTime = 101;
	}
}

