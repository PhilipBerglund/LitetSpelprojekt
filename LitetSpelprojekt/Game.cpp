#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
	:scene(windowWidth,windowHeight, window)
{
	//scene = Scene(windowWidth, windowHeight, window);
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
		scene.RenderShadowMap();
		scene.Render();
		inGameUI.Render(dt);
		break;
	
	case GameState::JOURNAL:
		scene.RenderShadowMap();
		scene.Render();
		inGameUI.Render(dt);
		break;

	case GameState::PAUSED:
		scene.RenderShadowMap();
		scene.Render();
		inGameUI.Render(dt);
		break;
	
	case GameState::CHAT:
		scene.Update(inGameUI, dt);
		scene.Render();
		inGameUI.Render(dt);
		break;

	case GameState::CLUE:
		scene.Update(inGameUI, dt);
		scene.Render();
		inGameUI.Render(dt);
		break;

	case GameState::END:
		inGameUI.Render(dt);
		break;
	}

	Graphics::Get2DRenderTarget().EndDraw();
	Graphics::EndFrame();
}