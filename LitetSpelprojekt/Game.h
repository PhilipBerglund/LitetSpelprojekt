#pragma once
#include "Scene.h"
#include "InGameUI.h"
#include "MainMenu.h"
#include "Timer.h"

enum class GameState { MAINMENU, INGAME, OPENJOURNAL, PAUSED, END };

class Game
{
private:
	Scene scene;
	InGameUI inGameUI;
	MainMenu mainMenu;
	InputHandler input;
	GameState state = GameState::MAINMENU;
	Timer timer;
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	GameState GetState() const { return this->state; };
	void SetState(GameState state) { this->state = state; };

	void CatchInput(unsigned char key, bool down = true);
	void CatchInput(std::pair<int, int> pos, bool down = true);
	void CatchRawInput(std::pair<int, int> delta) { input.OnRawDelta(delta.first, delta.second); };

	void Render(float dt);
};