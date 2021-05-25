#include "Game.h"



Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
	:scene(windowWidth,windowHeight, window)
{
	Event::Bind(this, EventType::RESET);
}

void Game::Reset()
{
	scene.Reset(inGameUI);
}

void Game::OnEvent()
{
	Reset();
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
		inGameUI.Render(dt);
		break;
	
	case GameState::JOURNAL: case GameState::PAUSED:
		scene.Render();
		inGameUI.Render(dt);
		break;
	
	case GameState::CHAT: case GameState::CLUE:
		scene.Update(inGameUI, dt);
		scene.Render();
		inGameUI.Render(dt);
		break;

	case GameState::END_WIN: case GameState::END_LOSS:
		mainMenu.Render();
		break;
	}

	Graphics::Get2DRenderTarget().EndDraw();
	Graphics::EndFrame();
}