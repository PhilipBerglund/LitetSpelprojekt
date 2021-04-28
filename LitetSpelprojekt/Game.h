#pragma once

#include "Scene.h"
#include "InGameUI.h"
#include "MainMenu.h"
#include "Timer.h"
#include "GameSettings.h"

class Game
{
private:
	Scene scene;
	InGameUI inGameUI;
	MainMenu mainMenu;
	Timer timer;
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);

	void Render(float dt);
};