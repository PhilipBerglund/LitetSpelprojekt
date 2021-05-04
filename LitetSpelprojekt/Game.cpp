#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	scene = Scene(windowWidth, windowHeight, window);
}

void Game::Render(float dt)
{
	Graphics::BeginFrame();
	Graphics::Get2DRenderTarget().BeginDraw();

	switch (GameSettings::GetState())
	{
	case GameState::MAINMENU:
		mainMenu.Render();
		break;

	case GameState::INGAME:
		SetCursorPos((int)Window::GetWidth() / 2, (int)Window::GetHeight() / 2);
		scene.Update(inGameUI, dt);
		scene.Render();
		inGameUI.Render();
		break;
	
	case GameState::JOURNAL:
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

	Graphics::Get2DRenderTarget().EndDraw();
	Graphics::EndFrame();
}