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

void Scenario::Run(InputHandler& input, float dt, Camera& camera)
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
		}
	}
}

