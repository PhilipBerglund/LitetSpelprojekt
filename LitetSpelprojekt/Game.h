#pragma once

#include "Scene.h"
#include "InGameUI.h"
#include "MainMenu.h"
#include "Timer.h"
#include "GameSettings.h"

class Game
{
	friend class Event;
private:
	Scene scene;
	InGameUI inGameUI;
	MainMenu mainMenu;
	Timer timer;
private:
	void OnEvent();
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	void Reset();
	void Render(float dt);
};