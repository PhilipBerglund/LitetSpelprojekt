#pragma once
#include "Scene.h"
#include "MainMenu.h"

enum class GameState { MAINMENU, INGAME, PAUSED, END };

class Game
{
private:
	Graphics graphics;
	Scene scene;
	MainMenu mainMenu;
	InputHandler input;
	GameState state = GameState::INGAME;
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	GameState GetState() const { return this->state; };

	void CatchInput(unsigned char key, bool down = true);
	void CatchInput(std::pair<int, int> pos, bool down = true);
	void CatchRawInput(std::pair<int, int> delta) { input.OnRawDelta(delta.first, delta.second); };

	void Render(float dt);
};