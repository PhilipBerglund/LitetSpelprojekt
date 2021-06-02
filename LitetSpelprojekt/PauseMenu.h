#pragma once

#include "Window.h"
#include "Event.h"
#include "GameSettings.h"
#include "UI.h"
#include "SoundHandler.h"

class PauseMenu
{
private:
	enum class State { MAIN, HELP, OPTIONS };
private:
	std::vector<Button> buttons;
	std::vector<Image> buttonArrows;

	State state = State::MAIN;

	Button helpButton;
	Button optionsButton;
	Button exitButton;
	Button resumeButton;

	Image background;

	//HELP PAGE
	Image helpBackground;

	//OPTIONS PAGE
	Image optionsBackground;

	Image slider;
	Button sliderButton;

	float sliderMinX;
	float sliderMaxX;
	bool leftDown = false;

	Button checkmarkButton;
	Image checkmark;

	Button backButton;
	Image backButtonArrows;

	bool resume = false;
public:
	PauseMenu()
	{
		Event::Bind(this, EventType::LEFTCLICK);
		Event::Bind(this, EventType::LEFTUP);
		Event::Bind(this, EventType::MOUSEMOVE);

		D2D_VECTOR_2F center = { (float)Window::GetCenter().first, (float)Window::GetCenter().second };

		helpButton = Button(true, { center.x, center.y - 92 }, 100.0f, 50.0f);
		optionsButton = Button(true, { center.x + 2.0f, center.y }, 140.0f, 50.0f);
		exitButton = Button(true, { center.x, center.y + 92 }, 90.0f, 50.0f);
		resumeButton = Button(true, { center.x, center.y + 203.0f }, 120.0f, 50.0f);

		buttonArrows.push_back(Image(L"./UI/HelpArrowsSmall.png", 1.0f, false, helpButton.position));
		buttonArrows.push_back(Image(L"./UI/OptionsArrowsSmall.png", 1.0f, false, optionsButton.position));
		buttonArrows.push_back(Image(L"./UI/ExitArrowsSmall.png", 1.0f, false, exitButton.position));
		buttonArrows.push_back(Image(L"./UI/OptionsArrowsSmall.png", 1.0f, false, resumeButton.position));

		buttons.push_back(helpButton);
		buttons.push_back(optionsButton);
		buttons.push_back(exitButton);
		buttons.push_back(resumeButton);

		background = Image(L"./UI/PauseMenu.png", 1.0f, true, center);

		//HELP
		helpBackground = Image(L"./UI/HowToPlayMenu.png", 1.0f, true, center);

		//OPTIONS
		optionsBackground = Image(L"./UI/OptionsMenu.png", 1.0f, true, center);
		slider = Image(L"./UI/SliderThing.png", 1.0f, true, { center.x, center.y + 20});
		sliderButton = Button(true, slider.position, slider.width, slider.height);
		checkmark = Image(L"./UI/Checkmark.png", 0.8f, false, { center.x + 50, center.y + 120 });
		checkmarkButton = Button(true, checkmark.position, checkmark.width, checkmark.height);

		sliderMinX = slider.position.x - 175;
		sliderMaxX = slider.position.x + 155;

		float currentVolume = SoundHandler::GetVolume();
		float sliderX = currentVolume * (sliderMaxX - sliderMinX) + sliderMinX;

		slider.SetPosition(sliderX, slider.position.y);
		sliderButton.SetPosition(slider.position.x, slider.position.y);

		backButton = Button(true, { Window::GetWidth() - 150, Window::GetHeight() - 80 }, 100.0f, 50.0f);
		backButtonArrows = Image(L"./UI/ExitArrows.png", 1.0f, true, backButton.position);
	}

	void Draw()
	{
		if (state == State::MAIN)
		{
			background.Draw();

			for (auto& arrow : buttonArrows)
			{
				if (arrow.visible)
					arrow.Draw();
			}
		}

		if (state == State::HELP)
		{
			SoundHandler::SetVolume(0.6f);
			helpBackground.Draw();

			if (backButtonArrows.visible)
				backButtonArrows.Draw();
		}

		if (state == State::OPTIONS)
		{
			optionsBackground.Draw();
			slider.Draw();

			if (checkmark.visible)
				checkmark.Draw();

			if (backButtonArrows.visible)
				backButtonArrows.Draw();
		}
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::PAUSED)
			return;
		
		std::pair<int, int> pos = Window::GetMousePos();

		switch (Event::GetCurrentEvent())
		{
		case EventType::MOUSEMOVE:
			if (state == State::MAIN)
			{
				for (unsigned int i = 0; i < buttons.size(); ++i)
				{
					if (buttons[i].OnHover(pos.first, pos.second))
						buttonArrows[i].SetVisibility(true);
					else
						buttonArrows[i].SetVisibility(false);
				}
			}
			
			if (state == State::HELP || state == State::OPTIONS)
			{
				if (backButton.OnHover(pos.first, pos.second))
					backButtonArrows.SetVisibility(true);
				else
					backButtonArrows.SetVisibility(false);
			}

			if (state == State::OPTIONS)
			{
				if (leftDown && !SoundHandler::IsMuted())
				{
					int newX = 0;;

					if (pos.first < sliderMinX)
						newX = sliderMinX;

					if (pos.first > sliderMaxX)
						newX = sliderMaxX;

					if (pos.first > sliderMinX && pos.first < sliderMaxX)
						newX = pos.first;

					slider.SetPosition(newX, slider.position.y);
					sliderButton.SetPosition(slider.position.x, slider.position.y);

					float volumePercent = (newX - sliderMinX) / (sliderMaxX - sliderMinX);
					SoundHandler::SetVolume(volumePercent);
				}
			}

			break;

		case EventType::LEFTCLICK:
		{
			if (state == State::MAIN)
			{
				if (optionsButton.OnClick(pos.first, pos.second))
					state = State::OPTIONS;

				if (helpButton.OnClick(pos.first, pos.second))
					state = State::HELP;

				if (exitButton.OnClick(pos.first, pos.second))
					GameSettings::SetState(GameState::MAINMENU);

				if (resumeButton.OnClick(pos.first, pos.second))
					resume = true;
			}

			if (state == State::HELP || state == State::OPTIONS)
				if (backButton.OnClick(pos.first, pos.second))
					state = State::MAIN;

			if (state == State::OPTIONS)
			{
				if (sliderButton.OnClick(pos.first, pos.second))
					leftDown = true;

				if (checkmarkButton.OnClick(pos.first, pos.second))
				{
					if (!SoundHandler::IsMuted())
					{
						SoundHandler::Mute();
						SoundHandler::SetVolume(0.0f);
						slider.SetPosition(sliderMinX, slider.position.y);
						sliderButton.SetPosition(slider.position.x, slider.position.y);
					}

					else
						SoundHandler::Unmute();

					checkmark.ToggleVisibility();
				}
			}

			break;

		case EventType::LEFTUP:
		{
			leftDown = false;
			break;
		}
		}
		}
	}

	bool Resume()
	{
		return resume;
	}

	void Show()
	{
		state = State::MAIN;
		resume = false;

		float currentVolume = SoundHandler::GetVolume();
		float sliderX = currentVolume * (sliderMaxX - sliderMinX) + sliderMinX;

		checkmark.SetVisibility(SoundHandler::IsMuted());
		slider.SetPosition(sliderX, slider.position.y);
		sliderButton.SetPosition(slider.position.x, slider.position.y);
	}
};