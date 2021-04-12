#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!graphics.Initialize(windowWidth, windowHeight, window))
		std::cout << "FAILED TO INITIALIZE GRAPHICS" << std::endl;

	scene = Scene(graphics, windowWidth, windowHeight, window);
	state = GameState::INGAME;
}

GameState Game::GetState() const
{
	return this->state;
}

void Game::CatchInput(unsigned char key, bool down)
{
	if (down)
		input.OnkeyPressed(key);
	else
		input.OnkeyReleased(key);
}

void Game::CatchInput(std::pair<float, float> pos, bool down)
{
	if (down)
		input.OnLeftPressed(pos.first, pos.second);
		
	else
		input.OnLeftRelease();
}

void Game::CatchRawInput(std::pair<float, float> delta)
{
	input.OnRawDelta(delta.first, delta.second);
}

void Game::Render(float dt)
{
	switch (state)
	{
	case GameState::MAINMENU:
		break;

	case GameState::INGAME:
		scene.Update(input, dt);
		scene.Render(graphics);
		break;

	case GameState::PAUSED:
		break;
	}

	input.ClearRawData();
}