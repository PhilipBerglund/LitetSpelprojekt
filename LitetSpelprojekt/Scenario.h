#pragma once
#include "Model.h"
#include "Graphics.h"
#include "Camera.h"
#include "InGameUI.h"
#include "Event.h"

class Scene;

struct Information
{
	bool valueable = false;
	int numConnections = 0;
	std::string connections[2] = { "" };
	std::string rumours[3] = { "" };
	std::string info = "";
};

struct Suspect
{
	std::shared_ptr<Model> model;

	std::string name = "";
	int age = 0;
	float height = 0;
	float shoeSize = 0;
	Information information;
	std::string characteristics[3] = {""};

	Suspect(std::string path, XMFLOAT3 position);
};

struct Victim
{
	std::string name = "";
	int age = 0;
	float height = 0;
	float shoeSize = 0;
	Information information;
};

struct Clue
{
	std::shared_ptr<Model> model;
	std::string information;
	bool isFound = false;

	Clue(std::string path, XMFLOAT3 position);
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