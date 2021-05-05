#pragma once
#include "Model.h"
#include "Graphics.h"
#include "Camera.h"
#include "InGameUI.h"
#include "Event.h"

class Scene;

struct Information
{
	bool valueable;
	std::string connections[5];
	std::string info;
};

struct Suspect
{
	std::shared_ptr<Model> model;

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
	std::string information;
	bool found = false;

	Clue(std::string path);
};

class Scenario
{
private:
	int identifiedSuspects = 0;
public:
	std::vector<Clue> clues;
	std::vector<Suspect> suspects;
	std::vector<XMFLOAT3> clueLocations;

	Scenario() = default;
	Scenario(Scene& scene);
	void InitializeClueLocations();
	void SetRandomizedLocations();
	bool TempLoadClues(std::string path);
	void Update(Scene& scene, InGameUI& ui, Camera& camera);
};