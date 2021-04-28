#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!inGameUI.Initialize())
		Error("FAILED TO INITIALIZE IN GAME UI");

	if (!mainMenu.Initialize())
		Error("FAILED TO INITIALIZE MAIN MENU");

	scene = Scene(windowWidth, windowHeight, window);
}

void Game::Render(float dt)
{
	Graphics::BeginFrame();

	switch (GameSettings::GetState())
	{
	case GameState::MAINMENU:
		mainMenu.Render();
		break;

	case GameState::INGAME:
		scene.Update(inGameUI, dt);
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
}