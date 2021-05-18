#pragma once

#include "Event.h"
#include "UI.h"
#include "Window.h"
#include "GameSettings.h"

class ChatOverlay
{
private:
	float normalSpeed = 10.0f;
	float maxSpeed = 50.0f;
	float currentSpeed = 0.0f;

	Image base;
	Text speaker;
	Text text;

	int showingCharacters = 0;
	float timeBetweenChars = 0.5f;
	float timeSinceChar = 0.0f;
	bool speedUp = false;

	bool exit = false;

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

		//BASE
		base = Image(L"UI/ChatBase.png", 1.0f, true, {Window::GetWidth() / 2.0f, Window::GetHeight() - 170});

		//TEXT
		speaker = Text(L"", true, { Window::GetWidth() / 2.0f - 400, Window::GetHeight() - 250 }, 100);
		text = Text(L"", true, { Window::GetWidth() / 2.0f, Window::GetHeight() - 200 }, 800);
	}

	void Reset()
	{
		showingCharacters = 0;
		timeSinceChar = 0.0f;
		speedUp = false;
		exit = false;
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush, float dt)
	{
		Update(dt);

		base.Draw();
		speaker.Draw(format, brush);
		text.Draw(format, brush, showingCharacters);
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::CHAT)
			return;

		EventType currentEvent = Event::GetCurrentEvent();

		if (currentEvent == EventType::BUTTONDOWN)
			speedUp = true;

		else if (currentEvent == EventType::BUTTONUP)
			speedUp = false;

		if (currentEvent == EventType::BUTTONDOWN && text.GetSize() == showingCharacters)
			exit = true;
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
};