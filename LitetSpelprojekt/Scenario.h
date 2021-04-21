#pragma once
#include <vector>
#include "GameObject.h"
#include "Geometry.h"
#include "Model.h"
#include "Graphics.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Print.h"

enum ScenarioStates
{

};

struct Clue
{
	std::shared_ptr<Model> model;
	XMMATRIX worldMatrix;

	bool murderClue;
	bool found;
};

class Scenario
{

private: 
	bool succeeded;

public:
	Scenario() = default;
	//void LoadClues();
	bool TempLoadClues(Graphics& graphics, std::string path);
	void Run(InputHandler& input, float dt, Camera& camera);
	std::vector<Clue> clues;
};


