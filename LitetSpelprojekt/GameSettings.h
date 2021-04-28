#pragma once

enum class GameState { MAINMENU, INGAME, PAUSED, END };

class GameSettings
{
private:
	static GameState gameState;
public:
	static void SetState(GameState state)
	{
		gameState = state;
	}

	static GameState GetState()
	{
		return gameState;
	}
};