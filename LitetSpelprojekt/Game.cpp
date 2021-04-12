#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!graphics.Initialize(windowWidth, windowHeight, window))
		std::cout << "FAILED TO INITIALIZE GRAPHICS" << std::endl;

	scene = Scene(graphics, windowWidth, windowHeight, window);
}

void Game::CatchInput(unsigned char key, bool down)
{
	if (down)
		input.OnkeyPressed(key);
	else
		input.OnkeyReleased(key);
}

void Game::Update(float dt)
{
	scene.Update(input, dt);
}

void Game::Render()
{
	scene.Render(graphics);
}