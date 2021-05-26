#pragma once

#include "Event.h"
#include "UI.h"
#include "Window.h"
#include "GameSettings.h"

struct ChatWindow
{
	friend class Event;
private:
	bool active = false;

	Image background;
	Text name;
	Text text;

	float normalSpeed = 10.0f;
	float maxSpeed = 50.0f;
	float speed = 0.0f;

	int showingCharacters = 0;
	float timeBetweenChars = 0.5f;
	float timeSinceChar = 0.0f;
	bool speedUp = false;

	bool done = false;
private:
	void OnEvent()
	{
		if (!active)
			return;

		EventType event = Event::GetCurrentEvent();

		if (event == EventType::BUTTONDOWN)
			speedUp = true;

		else if (event == EventType::BUTTONUP)
			speedUp = false;
	}

public:
	ChatWindow()
	{
		Event::Bind(this, EventType::BUTTONDOWN);
		Event::Bind(this, EventType::BUTTONUP);

		background = Image(L"UI/ChatBase.png", 1.0f, true, { Window::GetWidth() / 2.0f, Window::GetHeight() - 170 });
		name = Text(L"", true, { Window::GetWidth() / 2.0f - 400, Window::GetHeight() - 250 }, 200);
		text = Text(L"", true, { Window::GetWidth() / 2.0f, Window::GetHeight() - 200 }, 800);
	}

	void SetUp(std::string name, std::string text)
	{
		std::wstring wstr(text.begin(), text.end());
		this->text.SetString(wstr);

		std::wstring wname(name.begin(), name.end());
		this->name.SetString(wname);
	}

	void UpdateText(float dt)
	{
		if (speedUp)
			speed = maxSpeed;

		else
			speed = normalSpeed;

		timeSinceChar += dt * speed;

		if (timeSinceChar >= timeBetweenChars)
		{
			timeSinceChar = 0.0f;

			if (showingCharacters < text.GetSize())
				showingCharacters++;
		}

		if (showingCharacters == text.GetSize())
			done = true;
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		background.Draw();
		name.Draw(format, brush);
		text.Draw(format, brush, showingCharacters);
	}

	void Reset()
	{
		done = false;
		speedUp = false;
		timeSinceChar = 0.0f;
		showingCharacters = 0;
		speed = normalSpeed;
	}

	bool IsDone() const { return this->done; }

	bool IsActive() const { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { this->active = false; }
};

struct ChoiceWindow
{
	friend class Event;
public:
	enum class Choice { NONE = -1, CONVERSE, CONFRONT };

private:
	bool active = false;

	Image background;
	Button converseButton;
	Button confrontButton;
	Image converseArrows;
	Image confrontArrows;

	Choice choice = Choice::NONE;
private:
	void OnEvent()
	{
		if (!active)
			return;

		EventType event = Event::GetCurrentEvent();
		std::pair<int, int> pos = Window::GetMousePos();

		if (event == EventType::MOUSEMOVE)
		{
			//CONVERSE
			if (converseButton.OnHover(pos.first, pos.second))
				converseArrows.SetVisibility(true);
			else
				converseArrows.SetVisibility(false);

			//CONFRONT
			if (confrontButton.OnHover(pos.first, pos.second))
				confrontArrows.SetVisibility(true);
			else
				confrontArrows.SetVisibility(false);
		}

		if (event == EventType::LEFTCLICK)
		{
			if (converseButton.OnClick(pos.first, pos.second))
				choice = Choice::CONVERSE;

			else if (confrontButton.OnClick(pos.first, pos.second))
				choice = Choice::CONFRONT;

			else
				choice = Choice::NONE;
		}
	}

public:
	ChoiceWindow()
	{
		Event::Bind(this, EventType::MOUSEMOVE);
		Event::Bind(this, EventType::LEFTCLICK);

		background = Image(L"UI/ConfrontTab.png", 1.0f, true, { Window::GetWidth() / 2, Window::GetHeight() / 2 });
		converseButton = Button(true, { background.position.x, background.position.y - 20 }, 180, 30);
		confrontButton = Button(true, { background.position.x, background.position.y + 55 }, 180, 30);
		converseArrows = Image(L"UI/ConfrontConverseArrows.png", 1.0f, false, converseButton.position);
		confrontArrows = Image(L"UI/ConfrontConverseArrows.png", 1.0f, false, confrontButton.position);
	}

	void Draw()
	{
		background.Draw();

		#ifdef _DEBUG
		converseButton.DrawBounds();
		confrontButton.DrawBounds();
		#endif

		if (converseArrows.visible)
			converseArrows.Draw();

		if (confrontArrows.visible)
			confrontArrows.Draw();
	}

	void Reset()
	{
		active = false;
		choice = Choice::NONE;
	}

	Choice GetChoice() const { return this->choice; }

	bool IsActive() const { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { this->active = false; }
};

struct ConfirmWindow
{
	friend class Event;
public:
	enum class Choice { NONE = -1, YES, NO };

private:
	bool active = false;

	Image background;
	Button yesButton;
	Button noButton;
	Image yesArrows;
	Image noArrows;

	Choice choice = Choice::NONE;
private:
	void OnEvent()
	{
		if (!active)
			return;

		EventType event = Event::GetCurrentEvent();
		std::pair<int, int> pos = Window::GetMousePos();

		if (event == EventType::MOUSEMOVE)
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

		if (event == EventType::LEFTCLICK)
		{
			if (yesButton.OnClick(pos.first, pos.second))
				choice = Choice::YES;

			else if (noButton.OnClick(pos.first, pos.second))
				choice = Choice::NO;

			else
				choice = Choice::NONE;
		}
	}

public:
	ConfirmWindow()
	{
		Event::Bind(this, EventType::MOUSEMOVE);
		Event::Bind(this, EventType::LEFTCLICK);

		background = Image(L"UI/ConfirmationTab.png", 1.0f, true, { Window::GetWidth() / 2, Window::GetHeight() / 2 });
		yesButton = Button(true, { background.position.x - 85, background.position.y + 55 }, 60, 30);
		noButton = Button(true, { yesButton.position.x + 170, yesButton.position.y }, 50, 30);
		yesArrows = Image(L"UI/YesArrows.png", 1.0f, false, { yesButton.position.x + 3, yesButton.position.y});
		noArrows = Image(L"UI/NoArrows.png", 1.0f, false, { noButton.position.x - 2, noButton.position.y });
	}

	void Draw()
	{
		background.Draw();

	#ifdef _DEBUG
	yesButton.DrawBounds();
	noButton.DrawBounds();
	#endif

		if (yesArrows.visible)
			yesArrows.Draw();

		if (noArrows.visible)
			noArrows.Draw();
	}

	void Reset()
	{
		active = false;
		choice = Choice::NONE;
	}

	Choice GetChoice() const { return this->choice; }

	bool IsActive() const { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { this->active = false; }
};

class ChatOverlay
{
	friend class Event;
private:
	enum class State { NONE = -1, CHOOSE, CHAT, CONFIRM };

private:
	State state = State::NONE;

	ChoiceWindow choiceWin;
	ChatWindow chatWin;
	ConfirmWindow confirmWin;

	bool done = false;
	bool active = false;
	bool convict = false;
private:
	void OnEvent()
	{
		if (!active)
			return;

		if (chatWin.IsDone() && state == State::CHAT)
			done = true;
	}

	void Reset()
	{
		convict = false;
		active = false;
		done = false;
		choiceWin.Reset();
		chatWin.Reset();
		confirmWin.Reset();
	}
public:
	ChatOverlay() { Event::Bind(this, EventType::LEFTCLICK); }

	void Update(float dt)
	{
		if (state == State::CHAT)
			chatWin.UpdateText(dt);

		if (state == State::CHOOSE)
		{
			State newState = State::NONE;
			if (choiceWin.GetChoice() == ChoiceWindow::Choice::CONVERSE)
			{
				newState = State::CHAT;
				chatWin.Activate();
				choiceWin.Deactivate();
			}

			if (choiceWin.GetChoice() == ChoiceWindow::Choice::CONFRONT)
			{
				newState = State::CONFIRM;
				confirmWin.Activate();
				choiceWin.Deactivate();
			}

			if (newState != State::NONE)
				state = newState;
		}

		if (state == State::CONFIRM)
		{
			if (confirmWin.GetChoice() == ConfirmWindow::Choice::YES)
			{
				Reset();
				convict = true;
				done = true;
			}

			if (confirmWin.GetChoice() == ConfirmWindow::Choice::NO)
			{
				done = true;
			}
		}
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		switch (state)
		{
		case State::CHOOSE:
			choiceWin.Draw();
			break;

		case State::CHAT:
			chatWin.Draw(format, brush);
			break;

		case State::CONFIRM:
			confirmWin.Draw();
			break;
		}
	}

	void SetUp(std::string name, std::string string, bool firstInteraction = false)
	{
		Reset();

		if (firstInteraction)
		{
			state = State::CHAT;
			chatWin.Activate();
		}
		
		else
		{
			state = State::CHOOSE;
			choiceWin.Activate();
		}

		chatWin.SetUp(name, string);
		Activate();
		GameSettings::SetState(GameState::CHAT);
	}

	bool IsDone() { return this->done; }

	bool IsActive() const { return this->active; }
	void Activate() { this->active = true; }
	void Deactivate() { Reset(); }

	bool Convict() { return this->convict; }

	bool ShowCursor() 
	{ 
		if (state == State::CHOOSE || state == State::CONFIRM)
			return true; 
		return false; 
	}
};