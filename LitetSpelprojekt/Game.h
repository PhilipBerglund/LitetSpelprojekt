#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "InputHandler.h"

enum class GameState { MAINMENU, INGAME, PAUSED };

class Game
{
private:
	Scene scene;
	Graphics graphics;
	InputHandler input;
	GameState state;
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);
	GameState GetState() const;

	void CatchInput(unsigned char key, bool down = true);
	void CatchInput(std::pair<float, float> pos, bool down = true);
	void CatchRawInput(std::pair<float, float> delta);
	void Render(float dt);
};