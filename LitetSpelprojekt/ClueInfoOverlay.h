#pragma once
#include "Event.h"
#include "UI.h"
#include "Window.h"
#include "GameSettings.h"

struct ClueInfoOverlay
{
	friend class Event;
private:
	bool active = false;
	bool done = false;

	Image background;
	Text clueNumber;
	Text clueInformation;

private:
	void OnEvent()
	{
		if (!active)
			return;

		done = true;
	}

public:
	ClueInfoOverlay()
	{
		Event::Bind(this, EventType::BUTTONDOWN);

		background = Image(L"UI/ClueInfo.png", 1.0f, true, { Window::GetWidth() / 2, Window::GetHeight() / 2 });
		clueNumber = Text(L"", true, { background.position.x, background.position.y - 80 });
		clueInformation = Text(L"", true, { background.position.x + 10, background.position.y - 50 });
	}

	void Draw(IDWriteTextFormat& headerFormat, IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		background.Draw();
		clueNumber.Draw(headerFormat, brush);
		clueInformation.Draw(format, brush);
	}

	void SetUp(int clueNumber, std::string clueInformation)
	{
		std::wstring num = L"Clue "+ std::to_wstring(clueNumber) + L"#";
		this->clueNumber = Text(num, true, this->clueNumber.position, num.size() * 40);

		std::wstring info(clueInformation.begin(), clueInformation.end());
		this->clueInformation = Text(info, true, this->clueInformation.position, info.size() * 3.5f);
		//this->clueInformation.SetString(info);
		//this->clueInformation.SetPosition(this->clueInformation.position.x - (info.size() * 30.0f / 2.0f), this->clueInformation.position.y);
		Activate();
		GameSettings::SetState(GameState::CLUE);
	}

	bool IsDone() { return this->done; }

	bool IsActive() { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { this->active = false; done = false; }
};