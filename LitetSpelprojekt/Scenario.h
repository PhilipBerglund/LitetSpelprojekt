#pragma once
#include <vector>
#include "GameObject.h"
#include "Geometry.h"
#include "Model.h"
#include "Graphics.h"
#include "InputHandler.h"
#include "Camera.h"
#include "InGameUI.h"
#include "Event.h"

enum ScenarioStates {};

struct Information
{
	bool valueable;
	std::string connections[5];
	std::string info;
};

struct Suspect
{
	std::string name = "UNKNOWN";
	int age;
	Information information;
};

struct Victim
{
	std::string name = "UNKNOWN";
	int age;
	Information information;
};

struct Clue
{
	std::shared_ptr<Model> model;
	XMMATRIX worldMatrix;

	bool murderClue;
	bool found;
	std::string information;
};

class Scenario
{
private: 
	bool finished = false;
public:
	Victim victim;
	std::vector<Clue> clues;
	std::vector<Suspect> suspects;
	std::vector<XMFLOAT3> clueLocations;

	Scenario();
	void InitializeClueLocations();
	void SetRandomizedLocations();
	bool TempLoadClues(std::string path);
	void Run(InGameUI& ui, Camera& camera);
};