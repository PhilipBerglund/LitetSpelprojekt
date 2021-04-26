#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!inGameUI.Initialize())
		Error("FAILED TO INITIALIZE IN GAME UI");

	if (!mainMenu.Initialize())
		Error("FAILED TO INITIALIZE MAIN MENU");

	scene = Scene(windowWidth, windowHeight, window);
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
	input.OnMouseMove(pos.first, pos.second);

	if (down)
		input.OnLeftPressed(pos.first, pos.second);

	else
		input.OnLeftRelease();

	switch (state)
	{
	case GameState::MAINMENU:
		mainMenu.GetMousePos(input.GetHoveringPosition().first, input.GetHoveringPosition().second);
		if (down)
			SetState((GameState)mainMenu.GetInput(pos.first, pos.second));
		break;
	case GameState::INGAME:
		if (down)
			inGameUI.GetInput(pos.first, pos.second);
		break;
	}
}

void Game::Render(float dt)
{
	Graphics::BeginFrame();

	switch (state)
	{
	case GameState::MAINMENU:
		mainMenu.Render();
		break;

	case GameState::INGAME:
		scene.Update(inGameUI, input, dt);
		scene.Render();
		inGameUI.Render();
		break;

	case GameState::PAUSED:
		scene.Render();
		inGameUI.Render();
		break;

	case GameState::END:
		break;
	}

	Graphics::EndFrame();
	input.ClearRawData();
}