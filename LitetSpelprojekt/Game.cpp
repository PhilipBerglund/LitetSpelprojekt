#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!graphics.Initialize(windowWidth, windowHeight, window))
		std::cerr << " > : ( " << std::endl;
}

void Game::LoadGame()
{
}

void Game::Update(float dt)
{
	XMMATRIX viewMatrix = camera.GetViewMatrix();
	XMMATRIX perspectiveMatrix = camera.GetPerspectiveMatrix();

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i].Update();
	}
	//UPDATE
	//MAYBE PARAM FOR KEYPRESS BUT IDK
}

void Game::Render()
{
	graphics.Render();
}

void Game::ShutDown()
{
	graphics.ShutDown();
}