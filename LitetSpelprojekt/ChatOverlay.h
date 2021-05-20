#pragma once

#include "Event.h"
#include "UI.h"
#include "Window.h"
#include "GameSettings.h"

class ChatOverlay
{
private:
	enum class TabState { CHOOSE, CHAT, CONFIRMATION, NONE };

	TabState state = TabState::NONE;

	float normalSpeed = 10.0f;
	float maxSpeed = 50.0f;
	float currentSpeed = 0.0f;

	//CHAT
	Image base;
	Text speaker;
	Text text;

	int showingCharacters = 0;
	float timeBetweenChars = 0.5f;
	float timeSinceChar = 0.0f;
	bool speedUp = false;

	bool exit = false;

	//CHOOSE
	Image choiceTab;
	Button converseButton;
	Button confrontButton;
	Image converseArrows;
	Image confrontArrows;

	//CONFIRMATION
	Image confirmTab;
	Button yesButton;
	Button noButton;
	Image yesArrows;
	Image noArrows;

	void Update(float dt)
	{
		if (speedUp)
			currentSpeed = maxSpeed;

		else
			currentSpeed = normalSpeed;

		timeSinceChar += dt * currentSpeed;

		if (timeSinceChar >= timeBetweenChars)
		{
			timeSinceChar = 0.0f;

			if (showingCharacters < text.GetSize())
				showingCharacters++;
		}
	}

public:
	ChatOverlay()
	{
		Event::Bind(this, EventType::BUTTONDOWN);
		Event::Bind(this, EventType::BUTTONUP);
		Event::Bind(this, EventType::MOUSEMOVE);
		Event::Bind(this, EventType::LEFTCLICK);

		//----CHAT-----
		//BASE
		base = Image(L"UI/ChatBase.png", 1.0f, true, {Window::GetWidth() / 2.0f, Window::GetHeight() - 170});

		//TEXT
		speaker = Text(L"", true, { Window::GetWidth() / 2.0f - 400, Window::GetHeight() - 250 }, 100);
		text = Text(L"", true, { Window::GetWidth() / 2.0f, Window::GetHeight() - 200 }, 800);

		//-----CHOOSE-----
		choiceTab = Image(L"UI/ConfrontTab.png", 1.0f, true, { Window::GetWidth() / 2, Window::GetHeight() / 2});
		converseButton = Button(true, { choiceTab.position.x, choiceTab.position.y - 20 }, 180, 30);
		confrontButton = Button(true, { choiceTab.position.x, choiceTab.position.y + 55 }, 180, 30);
		converseArrows = Image(L"UI/ConfrontConverseArrows.png", 1.0f, false, converseButton.position);
		confrontArrows = Image(L"UI/ConfrontConverseArrows.png", 1.0f, false, confrontButton.position);

		//-----CONFIRM-----
		confirmTab = Image(L"UI/ConfrontTab.png", 1.0f, true, choiceTab.position );
		yesButton = Button(true, { confirmTab.position.x - 90, confirmTab.position.y + 55 }, 60, 30);
		noButton =  Button(true, { yesButton.position.x + 90, yesButton.position.y }, 50, 30);
		yesArrows = Image(L"UI/YesArrows.png", 1.0f, false, yesButton.position);
		noArrows =  Image(L"UI/NoArrows.png", 1.0f, false, noButton.position);
	}

	void Reset()
	{
		state = TabState::CHOOSE;
		showingCharacters = 0;
		timeSinceChar = 0.0f;
		speedUp = false;
		exit = false;
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush, float dt)
	{
		switch (state)
		{
		case TabState::CHAT:
			Update(dt);
			base.Draw();
			speaker.Draw(format, brush);
			text.Draw(format, brush, showingCharacters);
			break;

		case TabState::CHOOSE:
			choiceTab.Draw();
			converseButton.DrawBounds();
			confrontButton.DrawBounds();

			if (converseArrows.visible)
				converseArrows.Draw();
			else if (confrontArrows.visible)
				confrontArrows.Draw();
			break;

		case TabState::CONFIRMATION:
			confirmTab.Draw();
			yesButton.DrawBounds();
			noButton.DrawBounds();

			if (yesArrows.visible)
				yesArrows.Draw();
			else if(noArrows.visible)
				noArrows.Draw();
			break;
		}
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::CHAT)
			return;

		EventType currentEvent = Event::GetCurrentEvent();

		if (state == TabState::CHOOSE || state == TabState::CONFIRMATION)
		{
			std::pair<int, int> pos = Window::GetMousePos();

			if (state == TabState::CHOOSE)
			{
				if (currentEvent == EventType::MOUSEMOVE)
				{
					//CONVERSE
					if (converseButton.OnHover(pos.first, pos.second))
						converseArrows.SetVisibility(true);
					else
						converseArrows.SetVisibility(false);

					//COFRONT
					if (confrontButton.OnHover(pos.first, pos.second))
						confrontArrows.SetVisibility(true);
					else
						confrontArrows.SetVisibility(false);
				}

				if (currentEvent == EventType::LEFTCLICK)
				{
					if (converseButton.OnClick(pos.first, pos.second))
						state = TabState::CHAT;

					if (confrontButton.OnClick(pos.first, pos.second))
						state = TabState::CONFIRMATION;
				}
			}

			if (state == TabState::CONFIRMATION)
			{
				if (currentEvent == EventType::MOUSEMOVE)
				{
					//YES
					if (yesButton.OnHover(pos.first, pos.second))
						yesArrows.SetVisibility(true);
					else
						yesArrows.SetVisibility(false);

					//NO
					if (noButton.OnHover(pos.first, pos.second))
						noArrows.SetVisibility(true);
					else
						noArrows.SetVisibility(false);
				}

				if (currentEvent == EventType::LEFTCLICK)
				{
					if (yesButton.OnClick(pos.first, pos.second))
						GameSettings::SetState(GameState::END);

					if (noButton.OnClick(pos.first, pos.second))
						exit = true;
				}
			}
		}

		else
		{
			if (currentEvent == EventType::BUTTONDOWN)
				speedUp = true;

			else if (currentEvent == EventType::BUTTONUP)
				speedUp = false;

			if (currentEvent == EventType::BUTTONDOWN && text.GetSize() == showingCharacters)
				exit = true;
		}
	}

	void SetUp(std::string name, std::string string)
	{
		Reset();

		std::wstring wstr(string.begin(), string.end());
		text.SetString(wstr);

		std::wstring wname(name.begin(), name.end());
		speaker.SetString(wname);
	}

	bool Exit() { return this->exit; }

	bool ShowCursor() { if (state == TabState::CHOOSE || state == TabState::CONFIRMATION) return true; return false; };
};