#pragma once
#include <vector>
#include "GameObject.h"
#include "Geometry.h"
#include "Model.h"

enum ScenarioStates
{

};

class Scenario
{
public:
	std::vector<std::shared_ptr<Model>> clues;
	bool succeeded;
	//void AddClue(Model clue);

	void LoadClues();

	void TempLoadClues(std::string path);

	void Run();
	void Initialize(const std::string& path);
};

struct Clue 
{

	Model *model;
	XMMATRIX worldMatrix;

	bool murderClue;
	bool found;

};
