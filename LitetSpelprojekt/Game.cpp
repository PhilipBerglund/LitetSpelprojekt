#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (graphics.Initialize(windowWidth, windowHeight, window))
	{
		if (!inGameUI.Initialize(graphics.Get2DRenderTarget()))
			Error("FAILED TO INITIALIZE IN GAME UI");
	}
	else
		Error("FAILED TO INITIALIZE GRAPHICS");

	scene = Scene(graphics, windowWidth, windowHeight, window);
}

void Game::CatchInput(unsigned char key, bool down)
{
	if (key == 'W' || key == 'A' || key == 'S' || key == 'D')
	{
		if (down)
		{
			input.OnkeyPressed(key);
			inGameUI.Notify(input);
		}

		else
			input.OnkeyReleased(key);
	}

	else if (timer.DeltaTime() > 0.05)
	{
		timer.Start();
		if (down)
		{
			input.OnkeyPressed(key);
			inGameUI.Notify(input);
		}

		else
			input.OnkeyReleased(key);
	}
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
			inGameUI.GetInput(pos.first, pos.second);
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
		if (input.KeyIsPressed(27))
			state = GameState::PAUSED;

		scene.Update(input, dt);
		scene.Render(graphics);
		inGameUI.Render(graphics.Get2DRenderTarget());
		break;

	case GameState::OPENJOURNAL:
		break;

	case GameState::PAUSED:
		if (input.KeyIsPressed(27))
			state = GameState::INGAME;

		scene.Render(graphics);
		inGameUI.Render(graphics.Get2DRenderTarget());
		break;

	case GameState::END:
		break;
	}

	graphics.EndFrame();
	input.ClearRawData();
}