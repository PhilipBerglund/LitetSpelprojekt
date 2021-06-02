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
	D2D_VECTOR_2F numPosition;
	D2D_VECTOR_2F textPosition;

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

		background = Image(L"./UI/ClueInfo.png", 1.0f, true, { Window::GetWidth() / 2, Window::GetHeight() / 2 });
		numPosition = { background.position.x, background.position.y - 80 };
		textPosition = { background.position.x, background.position.y - 50 };
		clueNumber = Text(L"", true, numPosition);
		clueInformation = Text(L"", true, textPosition);
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
		this->clueNumber = Text(num, true, numPosition, num.size() * 40);

		std::wstring info(clueInformation.begin(), clueInformation.end());
		this->clueInformation = Text(info, true, textPosition, 700);
	
		Activate();
		GameSettings::SetState(GameState::CLUE);
	}

	bool IsDone() { return this->done; }

	bool IsActive() { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { this->active = false; done = false; }
};