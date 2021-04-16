#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (graphics.Initialize(windowWidth, windowHeight, window))
	{
		if (!mainMenu.Initialize(graphics.GetFactory(), graphics.Get2DRenderTarget()))
			Error("FAILED TO INITIALIZE MAIN MENU");
	}
	else
		Error("FAILED TO INITIALIZE GRAPHICS");

	scene = Scene(graphics, windowWidth, windowHeight, window);
}

void Game::CatchInput(unsigned char key, bool down)
{
	if (down)
		input.OnkeyPressed(key);
	else
		input.OnkeyReleased(key);
}

void Game::CatchInput(std::pair<int, int> pos, bool down)
{
	if (down)
	{
		input.OnLeftPressed(pos.first, pos.second);
		switch(state)
		{
		case GameState::MAINMENU:
			break;
		case GameState::INGAME:
			mainMenu.GetInput(pos.first, pos.second);
			break;
		}
	}
			
	else
		input.OnLeftRelease();
}

void Game::Render(float dt)
{
	graphics.BeginFrame();

	switch (state)
	{
	case GameState::MAINMENU:
		break;

	case GameState::INGAME:
		scene.Update(input, dt);
		scene.Render(graphics);
		mainMenu.Render(graphics.GetFactory(), graphics.Get2DRenderTarget());
		break;

	case GameState::PAUSED:
		break;

	case GameState::END:
		break;
	}

	graphics.EndFrame();
	input.ClearRawData();
}