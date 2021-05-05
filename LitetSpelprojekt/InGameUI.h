#pragma once

#include "Journal.h"
#include "PauseMenu.h"

enum class CursorType { REGULAR, CROSS, CLUE, CHAT };

class InGameUI
{
private:
	std::vector<Button> buttons;
	std::vector<Image*> images;

	D2D1_RECT_F pauseOverlay;

	Image* journalIcon;
	Image* pauseMenuIcon;
	Image* newInformationNotation;
	
	Image* crossCursor;
	Image* clueCursor;
	Image cursor;
	CursorType currentCursorType;

	ComPtr<IDWriteTextFormat> textFormat;
	ComPtr<IDWriteTextLayout> textLayout;

	ComPtr<ID2D1SolidColorBrush> brush;
	ComPtr<ID2D1SolidColorBrush> opacityBrush;

	bool drawOverlay = false;
	float brushOpacity = 0.7f;

	std::wstring font = L"Gabriola";
	FLOAT fontSize = 40;

	PauseMenu pauseMenu;
private:
	void SwitchJournalState()
	{
		journal.Slide();
		Event::DispatchEvent(EventType::STATECHANGE);

		if (GameSettings::GetState() == GameState::JOURNAL)
		{
			GameSettings::SetState(GameState::INGAME);
			SetCursorType(CursorType::CROSS);
			drawOverlay = false;
		}

		else if (GameSettings::GetState() == GameState::INGAME)
		{
			GameSettings::SetState(GameState::JOURNAL);
			SetCursorType(CursorType::REGULAR);
			drawOverlay = true;
		}
	}

	void SwitchPauseState()
	{
		Event::DispatchEvent(EventType::STATECHANGE);
		pauseMenu.Show();

		if (GameSettings::GetState() == GameState::PAUSED)
		{
			GameSettings::SetState(GameState::INGAME);
			SetCursorType(CursorType::CROSS);
			cursor.ResetScale();
			drawOverlay = false;
		}

		else if (GameSettings::GetState() == GameState::INGAME)
		{
			GameSettings::SetState(GameState::PAUSED);
			SetCursorType(CursorType::REGULAR);
			cursor.SetScale(0.6f);
			drawOverlay = true;
		}
	}

	void Reset()
	{
		newInformationNotation->SetOpacity(1.0f);
		brushOpacity = 0.0f;
		drawOverlay = false;
	}
public:
	Journal journal;
public:
	~InGameUI()
	{
		delete journalIcon;
		delete pauseMenuIcon;
		delete newInformationNotation;

		delete crossCursor;
		delete clueCursor;
	}

	InGameUI()
	{
		currentCursorType = CursorType::CROSS;

		Event::Bind(this, EventType::E_DOWN);
		Event::Bind(this, EventType::MOUSEMOVE);
		Event::Bind(this, EventType::ESC);
		Event::Bind(this, EventType::RESET);

		D2D1_SIZE_F winSize = Graphics::Get2DRenderTarget().GetSize();
		pauseOverlay = { 0, 0, winSize.width, winSize.height };

		//IMAGES
		journalIcon= new Image(L"UI/MenuButton.png", 0.9f, true, { 50, 70 });
		pauseMenuIcon = new Image(L"UI/JournalButton.png", 0.9f, true, { 60, winSize.height - 90 });
		newInformationNotation = new Image(L"UI/InformationAdded.png", 0.9f, false, { winSize.width - 240, 40 });

		images.push_back(journalIcon);
		images.push_back(pauseMenuIcon);
		images.push_back(newInformationNotation);

		crossCursor = new Image(L"UI/CrossCursor.png", 1.0f, true, { winSize.width / 2, winSize.height / 2 });
		clueCursor = new Image(L"UI/ClueCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });
		cursor = Image(L"UI/RegularCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });

		images.push_back(crossCursor);
		images.push_back(clueCursor);

		Reset();

		//TEXT
		HRESULT hr = Graphics::GetWriteFactory().CreateTextFormat(font.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);

		if FAILED(hr)
		{
			Error("FAILED TO CREATE TEXT FORMAT");
			return;
		}

		hr = Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(62.0f / 255.0f, 52.0f / 255.0f, 49.0f / 255.0f), &brush);

		if FAILED(hr)
		{
			Error("FAILED TO CREATE SOLID COLOR BRUSH");
			return;
		}

		hr = Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(62.0f / 255.0f, 52.0f / 255.0f, 49.0f / 255.0f), &opacityBrush);

		if FAILED(hr)
		{
			Error("FAILED TO CREATE SOLID COLOR BRUSH");
			return;
		}

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	};

	void Render(float dt)
	{
		//PRE DRAW ADJUSTMENTS
		if (journal.Exit())
			SwitchJournalState();

		if (pauseMenu.Resume())
			SwitchPauseState();

		if (newInformationNotation->GetOpacity() <= 0)
			newInformationNotation->SetVisibility(false);

		else
			newInformationNotation->ReduceOpacity(0.5f * dt);
		
		if (drawOverlay)
		{
			if (brushOpacity < 0.7f)
			brushOpacity += 3.0f * dt;
		}

		else
		{
			if (brushOpacity > 0.0f)
				brushOpacity -= 2.0f * dt;
		}

		opacityBrush->SetOpacity(brushOpacity);

		//DRAWING
		for (auto& image : images)
		{
			if (image->visible)
				image->Draw();
		}

		Graphics::Get2DRenderTarget().FillRectangle(pauseOverlay, opacityBrush.Get());

		journal.Draw(*textFormat.Get(), *brush.Get(), dt);

		if (GameSettings::GetState() == GameState::PAUSED)
			pauseMenu.Draw();

		if (cursor.visible)
			cursor.Draw();
	}

	void OnEvent()
	{
		std::pair<int, int> pos = Window::GetMousePos();
		EventType currentEvent = Event::GetCurrentEvent();
		GameState currentState = GameSettings::GetState();

		if (currentState == GameState::JOURNAL || currentState == GameState::INGAME)
		{
			if (Event::GetCurrentEvent() == EventType::E_DOWN)
			{
				SwitchJournalState();
				cursor.SetPosition((float)pos.first, (float)pos.second + 30);
			}
		}

		if (currentState == GameState::PAUSED || currentState == GameState::INGAME)
		{
			if (currentEvent == EventType::ESC)
			{
				SwitchPauseState();
				cursor.SetPosition((float)pos.first, (float)pos.second + 30);
			}
		}

		if (currentEvent == EventType::RESET)
			Reset();

		if (currentEvent == EventType::MOUSEMOVE)
			cursor.SetPosition((float)pos.first, (float)pos.second + 30);
	}

	void ShowNotification()
	{
		newInformationNotation->SetVisibility(true);
		newInformationNotation->SetOpacity(1.0f);
	}

	void SetCursorType(CursorType type)
	{
		if (type != currentCursorType)
		{
			switch (type)
			{
			case CursorType::CROSS:
				currentCursorType = CursorType::CROSS;
				crossCursor->SetVisibility(true);
				clueCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;

			case CursorType::CLUE:
				currentCursorType = CursorType::CLUE;
				clueCursor->SetVisibility(true);
				crossCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;

			case CursorType::REGULAR:
				currentCursorType = CursorType::REGULAR;
				cursor.SetVisibility(true);
				crossCursor->SetVisibility(false);
				clueCursor->SetVisibility(false);
				break;

			};
		}
	}
};