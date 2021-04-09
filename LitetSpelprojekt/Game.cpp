#include "Game.h"

Game::Game(HWND window, UINT windowWidth, UINT windowHeight)
{
	if (!graphics.Initialize(windowWidth, windowHeight, window))
		std::cout << "FAILED TO INITIALIZE GRAPHICS" << std::endl;

	camera = new Camera(XM_PIDIV4, (float)windowWidth / float(windowHeight), 0.01f, 100.0f, { 0.0f, 0.0f, -10.0f });
	LoadGame();
}

void Game::LoadGame()
{
	gameObjects.push_back(camera);
	gameObjects.push_back(new Model());
	
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i]->type() == Type::MODEL)
			dynamic_cast<Model*>(gameObjects[i])->Initialize(graphics.GetDevice(), "Troll.obj");
	}
}

void Game::Update(float dt)
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{		
		switch (gameObjects[i]->type())
		{
		case Type::CAMERA:
			dynamic_cast<Camera*>(gameObjects[i])->Update();
			break;

		case Type::MODEL:
			dynamic_cast<Model*>(gameObjects[i])->UpdateBuffers(graphics.GetDeviceContext());
			break;

		case Type::LIGHT:
			dynamic_cast<Light*>(gameObjects[i])->UpdateBuffers(graphics.GetDeviceContext());
			break;
		}
	}
}

void Game::Render()
{
	graphics.Render(gameObjects);
}

void Game::ShutDown()
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i] != nullptr)
			delete gameObjects[i];
	}

	graphics.ShutDown();
}