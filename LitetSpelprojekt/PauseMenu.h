#pragma once

#include "Window.h"
#include "Event.h"
#include "GameSettings.h"
#include "UI.h"

class PauseMenu
{
private:
	std::vector<Button> buttons;
	std::vector<Image> buttonArrows;

	Button helpButton;
	Button optionsButton;
	Button exitButton;
	Button resumeButton;

	Image background;

	bool resume = false;
public:
	PauseMenu()
	{
		Event::Bind(this, EventType::LEFTCLICK);
		Event::Bind(this, EventType::MOUSEMOVE);

		D2D_VECTOR_2F center = { (float)Window::GetCenter().first, (float)Window::GetCenter().second };

		helpButton = Button(true, { center.x, center.y - 92 }, 100.0f, 50.0f);
		optionsButton = Button(true, { center.x + 2.0f, center.y }, 140.0f, 50.0f);
		exitButton = Button(true, { center.x, center.y + 92 }, 90.0f, 50.0f);
		resumeButton = Button(true, { center.x, center.y + 203.0f }, 120.0f, 50.0f);

		buttonArrows.push_back(Image(L"UI/HelpArrowsSmall.png", 1.0f, false, helpButton.position));
		buttonArrows.push_back(Image(L"UI/OptionsArrowsSmall.png", 1.0f, false, optionsButton.position));
		buttonArrows.push_back(Image(L"UI/ExitArrowsSmall.png", 1.0f, false, exitButton.position));
		buttonArrows.push_back(Image(L"UI/OptionsArrowsSmall.png", 1.0f, false, resumeButton.position));

		buttons.push_back(helpButton);
		buttons.push_back(optionsButton);
		buttons.push_back(exitButton);
		buttons.push_back(resumeButton);

		background = Image(L"UI/PauseMenu.png", 1.0f, true, center);
	}

	void Draw()
	{
		background.Draw();

		for (auto& arrow : buttonArrows)
		{
			if (arrow.visible)
				arrow.Draw();
		}
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::PAUSED)
			return;
		
		std::pair<int, int> pos = Window::GetMousePos();

		switch (Event::GetCurrentEvent())
		{
		default:
			break;

		case EventType::MOUSEMOVE:
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
			if (optionsButton.OnClick(pos.first, pos.second))
				GameSettings::SetState(GameState::INGAME);

			if (helpButton.OnClick(pos.first, pos.second))
				GameSettings::SetState(GameState::INGAME);

			if (exitButton.OnClick(pos.first, pos.second))
				GameSettings::SetState(GameState::MAINMENU);

			if (resumeButton.OnClick(pos.first, pos.second))
				resume = true;

			break;
		}
		}
	}

	bool Resume()
	{
		return resume;
	}

	void Show()
	{
		resume = false;
	}
};