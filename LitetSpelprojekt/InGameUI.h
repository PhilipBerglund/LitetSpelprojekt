#pragma once

#include "Journal.h"
#include "PauseMenu.h"
#include "ChatOverlay.h"
#include "ClueInfoOverlay.h"

enum class CursorType { REGULAR, CROSS, CLUE, CHAT, NONE };

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
	Image* chatCursor;
	Image cursor;
	CursorType currentCursorType;

	Text dtText;
	ComPtr<IDWriteTextFormat> debugTextFormat;
	ComPtr<ID2D1SolidColorBrush> debugBrush;

	ComPtr<IDWriteTextFormat> textFormat;

	ComPtr<ID2D1SolidColorBrush> brush;
	ComPtr<ID2D1SolidColorBrush> whiteBrush;
	ComPtr<ID2D1SolidColorBrush> opacityBrush;

	bool drawOverlay = false;
	float brushOpacity = 0.7f;

	std::wstring font = L"Gabriola";
	FLOAT fontSize = 40;

	PauseMenu pauseMenu;

	bool gotNewInformation = false;
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

	void DeactivateChatOverlay()
	{
		chatOverlay.Deactivate();
		Event::DispatchEvent(EventType::STATECHANGE);
		GameSettings::SetState(GameState::INGAME);

		if (gotNewInformation)
		{
			newInformationNotation->SetVisibility(true);
			newInformationNotation->SetOpacity(1.0f);
		}
		
		gotNewInformation = false;
		SetCursorType(CursorType::CROSS);
	}

	void DeactivateClueOverlay()
	{
		clueOverlay.Deactivate();
		Event::DispatchEvent(EventType::STATECHANGE);
		GameSettings::SetState(GameState::INGAME);
		SetCursorType(CursorType::CROSS);
	}

	void Reset()
	{
		newInformationNotation->SetOpacity(1.0f);
		brushOpacity = 0.0f;
		drawOverlay = false;
	}

	void DrawOverlay(float dt)
	{
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
		Graphics::Get2DRenderTarget().FillRectangle(pauseOverlay, opacityBrush.Get());
	}

	void DrawNotification(float dt)
	{
		if (newInformationNotation->GetOpacity() <= 0)
			newInformationNotation->SetVisibility(false);

		else
			newInformationNotation->ReduceOpacity(0.5f * dt);
	}
public:
	Journal journal;
	ChatOverlay chatOverlay;
	ClueInfoOverlay clueOverlay;
public:
	~InGameUI()
	{
		delete journalIcon;
		delete pauseMenuIcon;
		delete newInformationNotation;

		delete crossCursor;
		delete clueCursor;
		delete chatCursor;
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
		chatCursor = new Image(L"UI/ChatCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });
		cursor = Image(L"UI/RegularCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });

		images.push_back(crossCursor);
		images.push_back(clueCursor);
		images.push_back(chatCursor);

		Reset();

		//TEX
		//DEBUG
		HRESULT hr = Graphics::GetWriteFactory().CreateTextFormat(font.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"", &debugTextFormat);
		if FAILED(hr)
		{
			Error("FAILED TO CREATE TEXT FORMAT");
			return;
		}

		hr = Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f), &debugBrush);
		if FAILED(hr)
		{
			Error("FAILED TO CREATE SOLID COLOR BRUSH");
			return;
		}

		//REGULAR
		hr = Graphics::GetWriteFactory().CreateTextFormat(font.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);
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

		hr = Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &whiteBrush);
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

		dtText = Text(L"", true, { Window::GetWidth() - 50, 0 }, 100);
	};

	void Render(float dt)
	{
		//PRE DRAW ADJUSTMENTS
		if (journal.Exit())
			SwitchJournalState();

		if (pauseMenu.Resume())
			SwitchPauseState();

		if (chatOverlay.IsDone())
			DeactivateChatOverlay();

		#ifdef _DEBUG
		dtText.SetString(std::to_wstring(dt * 1000.0f));
		dtText.Draw(*debugTextFormat.Get(), *debugBrush.Get());
		#endif

		//DRAWING
		for (auto& image : images)
		{
			if (image->visible)
				image->Draw();
		}

		DrawNotification(dt);
		DrawOverlay(dt);

		journal.Draw(*textFormat.Get(), *brush.Get(), dt);

		if (GameSettings::GetState() == GameState::PAUSED)
			pauseMenu.Draw();

		if (chatOverlay.IsActive())
		{
			chatOverlay.Update(dt);
			chatOverlay.Draw(*debugTextFormat.Get(), *whiteBrush.Get());

			if (chatOverlay.ShowCursor())
				SetCursorType(CursorType::REGULAR);

			else
				SetCursorType(CursorType::NONE);
		}

		if (clueOverlay.IsActive())
		{
			clueOverlay.Draw(*textFormat.Get(), *debugTextFormat.Get(), *whiteBrush.Get());

			if (clueOverlay.IsDone())
				DeactivateClueOverlay();
		}
			
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
		gotNewInformation = true;
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
				chatCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;

			case CursorType::CLUE:
				currentCursorType = CursorType::CLUE;
				clueCursor->SetVisibility(true);
				crossCursor->SetVisibility(false);
				chatCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;

			case CursorType::REGULAR:
				currentCursorType = CursorType::REGULAR;
				cursor.SetVisibility(true);
				crossCursor->SetVisibility(false);
				clueCursor->SetVisibility(false);
				chatCursor->SetVisibility(false);
				break;

			case CursorType::CHAT:
				currentCursorType = CursorType::CHAT;
				chatCursor->SetVisibility(true);
				crossCursor->SetVisibility(false);
				clueCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;

			case CursorType::NONE:
				currentCursorType = CursorType::NONE;
				chatCursor->SetVisibility(false);
				crossCursor->SetVisibility(false);
				clueCursor->SetVisibility(false);
				cursor.SetVisibility(false);
				break;
			};
		}
	}
};