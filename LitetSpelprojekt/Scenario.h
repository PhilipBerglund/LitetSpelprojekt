#pragma once
#include <vector>
#include "GameObject.h"
#include "Geometry.h"
#include "Model.h"
#include "Graphics.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Print.h"
#include "InGameUI.h"

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
	//float notificationTime = 101.0f;
public:
	std::vector<Clue> clues;
	std::vector<XMFLOAT3> clueLocations;

	Scenario() = default;
	//void LoadClues();
	void InitializeClueLocations();
	void SetRandomizedLocations();
	bool TempLoadClues(std::string path);
	void Run(InGameUI& ui, InputHandler& input, float dt, Camera& camera);
};