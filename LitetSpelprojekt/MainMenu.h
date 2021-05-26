#pragma once

#include "Window.h"
#include "Event.h"
#include "GameSettings.h"
#include "UI.h"

class MainMenu
{
private:
	enum class State { MAIN, HELP, OPTIONS, CREDITS, END };
private:
	State state = State::MAIN;

	//MAIN
	std::vector<Button> mainButtons;
	std::vector<Image> mainButtonArrows;

	Image mainBackground;

	Button playButton;
	Button optionsButton;
	Button helpButton;
	Button creditsButton;
	Button exitButton;

	//HELP
	Image helpBackground;

	//CREDITS
	Image creditsBackground;

	Button backButton;
	Image backButtonArrows;

	//END
	Image endWinBackground;
	Image endLossBackground;

	Button returnMainMenuButton;
	Image returnMainMenuButtonArrows;

	//ALWAYS
	Image cursor;

public:
	MainMenu()
	{
		Event::Bind(this, EventType::LEFTCLICK);
		Event::Bind(this, EventType::MOUSEMOVE);
		Event::Bind(this, EventType::STATECHANGE);

		D2D_VECTOR_2F center = { (float)Window::GetCenter().first, (float)Window::GetCenter().second };

		playButton = Button(true, { center.x - 5, center.y - 105 }, 135.0f, 50.0f);
		optionsButton = Button(true, { center.x - 5, center.y + 15 }, 230.0f, 50.0f);
		helpButton = Button(true, { center.x - 5,center.y + 150 }, 350.0f, 50.0f);
		creditsButton = Button(true, { center.x, center.y + 260 }, 215.0f, 50.0f);
		exitButton = Button(true, { center.x - 5, center.y + 380 }, 110.0f, 50.0f);

		mainButtonArrows.push_back(Image(L"UI/PlayArrows.png", 1.0f, false, playButton.position));
		mainButtonArrows.push_back(Image(L"UI/OptionsArrows.png", 1.0f, false, optionsButton.position));
		mainButtonArrows.push_back(Image(L"UI/HelpArrows.png", 1.0f, false, helpButton.position));
		mainButtonArrows.push_back(Image(L"UI/CreditsArrows.png", 1.0f, false, creditsButton.position));
		mainButtonArrows.push_back(Image(L"UI/ExitArrows.png", 1.0f, false, exitButton.position));

		mainButtons.push_back(playButton);
		mainButtons.push_back(optionsButton);
		mainButtons.push_back(helpButton);
		mainButtons.push_back(creditsButton);
		mainButtons.push_back(exitButton);

		mainBackground = Image(L"UI/MainMenu.png", 1.0f, true, center);

		//HELP
		helpBackground = Image(L"UI/HowToPlayMenu.png", 1.0f, true, center);

		//CREDITS
		creditsBackground = Image(L"UI/CreditsMenu.png", 1.0f, true, center);

		backButton = Button(true, { Window::GetWidth() - 150, Window::GetHeight() - 80 }, 100.0f, 50.0f);
		backButtonArrows = Image(L"UI/ExitArrows.png", 1.0f, true, backButton.position);

		//END
		endWinBackground = Image(L"UI/MissionComplete.png", 1.0f, true, center);
		endLossBackground = Image(L"UI/MissionFailed.png", 1.0f, true, center);

		returnMainMenuButton = Button(true, { center.x - 8, center.y + 160 }, 360, 40);
		returnMainMenuButtonArrows = Image(L"UI/ReturnToMainMenuArrows.png", 1.0f, true, returnMainMenuButton.position);
		
		//ALWAYS
		cursor = Image(L"UI/RegularCursor.png", 1.0f, true, { (float)Window::GetMousePos().first, (float)Window::GetMousePos().second + 30 });
	};

	void Render()
	{
		switch (state)
		{
		case State::MAIN:
			mainBackground.Draw();

			#ifdef _DEBUG
			for (auto& button : mainButtons)
				button.DrawBounds();
			#endif

			for (auto& arrows : mainButtonArrows)
				if (arrows.visible)
					arrows.Draw();
			break;
		case State::HELP: case State::CREDITS:
			if (state == State::HELP)
				helpBackground.Draw();
			else
				creditsBackground.Draw();

			#ifdef _DEBUG
			backButton.DrawBounds();
			#endif

			if (backButtonArrows.visible)
				backButtonArrows.Draw();
			break;

		case State::END:
			if (GameSettings::GetState() == GameState::END_WIN)
				endWinBackground.Draw();
			else
				endLossBackground.Draw();

			#ifdef _DEBUG
			returnMainMenuButton.DrawBounds();
			#endif

			if (returnMainMenuButtonArrows.visible)
				returnMainMenuButtonArrows.Draw();
		}

		cursor.Draw();
	}

	void OnEvent()
	{
		GameState newState = GameSettings::GetState();
		if (newState == GameState::END_LOSS || newState == GameState::END_WIN)
			state = State::END;

		if (!(GameSettings::GetState() == GameState::END_LOSS || GameSettings::GetState() == GameState::END_WIN || GameSettings::GetState() == GameState::MAINMENU))
			return;

		std::pair<int, int> pos = Window::GetMousePos();

		switch (Event::GetCurrentEvent())
		{
		default:
			break;

		case EventType::MOUSEMOVE:
			cursor.SetPosition((float)pos.first, (float)pos.second + 30);

			if (state == State::MAIN)
				for (unsigned int i = 0; i < mainButtons.size(); ++i)
				{
					if (mainButtons[i].OnHover(pos.first, pos.second))
						mainButtonArrows[i].SetVisibility(true);
					else
						mainButtonArrows[i].SetVisibility(false);
				}

			if (state == State::HELP || state == State::CREDITS)
			{
				if (backButton.OnHover(pos.first, pos.second))
					backButtonArrows.SetVisibility(true);
				else
					backButtonArrows.SetVisibility(false);
			}

			if (state == State::END)
			{
				if (returnMainMenuButton.OnHover(pos.first, pos.second))
					returnMainMenuButtonArrows.SetVisibility(true);
				else
					returnMainMenuButtonArrows.SetVisibility(false);
			}
				
			break;

		case EventType::LEFTCLICK:
			{
				if (state == State::MAIN)
				{
					if (playButton.OnClick(pos.first, pos.second))
					{
						GameSettings::SetState(GameState::INGAME);
						Event::DispatchEvent(EventType::STATECHANGE);
						Event::DispatchEvent(EventType::RESET);
					}

					if (optionsButton.OnClick(pos.first, pos.second))
						GameSettings::SetState(GameState::MAINMENU);

					if (helpButton.OnClick(pos.first, pos.second))
						state = State::HELP;

					if (creditsButton.OnClick(pos.first, pos.second))
						state = State::CREDITS;

					if (exitButton.OnClick(pos.first, pos.second))
						exit(0);

					break;
				}

				if (state == State::HELP || state == State::CREDITS)
					if (backButton.OnClick(pos.first, pos.second))
						state = State::MAIN;

				if (state == State::END)
				{
					if (returnMainMenuButton.OnClick(pos.first, pos.second))
					{
						state = State::MAIN;
						GameSettings::SetState(GameState::MAINMENU);
					}
				}
			}

			break;
		}	
	}
};