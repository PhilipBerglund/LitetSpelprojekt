#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "InputHandler.h"

class Game
{
private:
	Scene scene;
	Graphics graphics;
	InputHandler input;
public:
	Game(HWND window, UINT windowWidth, UINT windowHeight);

	void CatchInput(unsigned char key, bool down);
	void Update(float dt);
	void Render();
};