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