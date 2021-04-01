#pragma once
#include "Graphics.h"
#include "Model.h"
#include <vector>

class Game
{
private:
	Graphics graphics;
	std::vector<GameObject> objects;
	void LoadGame();
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	void Update(float dt);
	void Render();
	void ShutDown();
};