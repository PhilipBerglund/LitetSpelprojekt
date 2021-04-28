#pragma once

#include "Window.h"
#include "Event.h"
#include "GameSettings.h"
#include "UI.h"

class MainMenu
{
private:
	std::vector<Button> buttons;
	std::vector<Image> buttonArrows;

	Image background;

	Image cursor;

	Button playButton;
	Button optionsButton;
	Button helpButton;
	Button creditsButton;
	Button exitButton;
public:
	MainMenu()
	{
		Event::Bind(this, EventType::LEFTCLICK);
		Event::Bind(this, EventType::MOUSEMOVE);

		D2D_VECTOR_2F center = { (float)Window::GetCenter().first, (float)Window::GetCenter().second };

		playButton = Button(true, { center.x - 5, center.y - 105 }, 135.0f, 50.0f);
		optionsButton = Button(true, { center.x - 5, center.y + 15 }, 230.0f, 50.0f);
		helpButton = Button(true, { center.x - 5,center.y + 150 }, 350.0f, 50.0f);
		creditsButton = Button(true, { center.x, center.y + 260 }, 215.0f, 50.0f);
		exitButton = Button(true, { center.x - 5, center.y + 380 }, 110.0f, 50.0f);

		buttonArrows.push_back(Image(L"UI/PlayArrows.png", 1.0f, false, playButton.position));
		buttonArrows.push_back(Image(L"UI/OptionsArrows.png", 1.0f, false, optionsButton.position));
		buttonArrows.push_back(Image(L"UI/HelpArrows.png", 1.0f, false, helpButton.position));
		buttonArrows.push_back(Image(L"UI/CreditsArrows.png", 1.0f, false, creditsButton.position));
		buttonArrows.push_back(Image(L"UI/ExitArrows.png", 1.0f, false, exitButton.position));

		buttons.push_back(playButton);
		buttons.push_back(optionsButton);
		buttons.push_back(helpButton);
		buttons.push_back(creditsButton);
		buttons.push_back(exitButton);

		background = Image(L"UI/MainMenu.png", 1.0f, true, center);

		cursor = Image(L"UI/RegularCursor.png", 1.0f, true, { (float)Window::GetMousePos().first, (float)Window::GetMousePos().second + 30 });
	};

	void Render()
	{
		background.Draw();

		for (auto& arrows : buttonArrows)
		{
			if (arrows.visible)
				arrows.Draw();
		}

		cursor.Draw();
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::MAINMENU)
			return;

		std::pair<int, int> pos = Window::GetMousePos();

		switch (Event::GetCurrentEvent())
		{
		default:
			break;

		case EventType::MOUSEMOVE:
			cursor.SetPosition((float)pos.first, (float)pos.second + 30);

			for (unsigned int i = 0; i < buttons.size(); ++i)
			{
				if (buttons[i].OnHover(pos.first, pos.second))
					buttonArrows[i].SetVisibility(true);
				else
					buttonArrows[i].SetVisibility(false);
			}

			break;

		case EventType::LEFTCLICK:
			{
				if (playButton.OnClick(pos.first, pos.second))
				{
					GameSettings::SetState(GameState::INGAME);
					Event::DispatchEvent(EventType::RESET);
				}
					
				if (optionsButton.OnClick(pos.first, pos.second))
					GameSettings::SetState(GameState::MAINMENU);

				if (helpButton.OnClick(pos.first, pos.second))
					GameSettings::SetState(GameState::MAINMENU);

				if (creditsButton.OnClick(pos.first, pos.second))
					GameSettings::SetState(GameState::MAINMENU);

				if (exitButton.OnClick(pos.first, pos.second))
					exit(0);

				Event::DispatchEvent(EventType::STATECHANGE);
				break;
			}
		}	
	}
};