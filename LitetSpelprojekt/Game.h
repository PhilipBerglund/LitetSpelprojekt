#pragma once
#include "Graphics.h"
#include "Model.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Model.h"

class Game
{
private:
	Graphics graphics;
	Camera* camera;
	std::vector<GameObject*> gameObjects;
	void LoadGame();
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	void Update(float dt);
	void Render();
	void ShutDown();
};