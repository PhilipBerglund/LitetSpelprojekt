#pragma once
#include "Event.h"
#include "UI.h"
#include "Journal.h"


enum class CursorType { CROSS, CLUE, CHAT };

class InGameUI
{
private:
	std::vector<Button> buttons;
	std::vector<Image*> images;
	std::vector<Text*> texts;

	Image* journalButton;
	Image* pauseMenuButton;
	Image* newInformationNotation;

	Image* pauseMenu;

	Image* CrossCursor;
	Image* ClueCursor;
	CursorType currentCursorType = CursorType::CROSS;

	ComPtr<IDWriteTextFormat> textFormat;
	ComPtr<IDWriteTextLayout> textLayout;

	ComPtr<ID2D1SolidColorBrush> brush;
	std::wstring font = L"Gabriola";
	FLOAT fontSize = 50;
	Text* text;

	Journal journal;

public:
	InGameUI()
	{
		this->journalButton = nullptr;
		this->pauseMenuButton = nullptr;
		this->newInformationNotation = nullptr;
		this->pauseMenu = nullptr;
		this->CrossCursor = nullptr;
		this->ClueCursor = nullptr;
		this->text = nullptr;
	};

	bool Initialize()
	{
		Event::Bind(this, EventType::E_DOWN);
		D2D1_SIZE_F winSize = Graphics::Get2DRenderTarget().GetSize();

		//IMAGES
		journalButton = new Image(L"UI/MenuButton.png", 0.9f, true, { 50, 70 });
		pauseMenuButton = new Image(L"UI/JournalButton.png", 0.9f, true, { 60, winSize.height - 90 });
		newInformationNotation = new Image(L"UI/InformationAdded.png", 0.9f, false, { winSize.width - 240, 40 });
		pauseMenu = new Image(L"UI/PauseMenu.png", 0.9f, false, { winSize.width / 2, winSize.height / 2 });

		CrossCursor = new Image(L"UI/CrossCursor.png", 1.0f, true, { winSize.width / 2, winSize.height / 2 });
		ClueCursor = new Image(L"UI/ClueCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });

		images.push_back(journalButton);
		images.push_back(pauseMenuButton);
		images.push_back(newInformationNotation);
		images.push_back(pauseMenu);

		images.push_back(CrossCursor);
		images.push_back(ClueCursor);

		//TEXT
		HRESULT hr = Graphics::GetWriteFactory().CreateTextFormat(font.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);

		if FAILED(hr)
		{
			Error("FAILED TO CREATE TEXT FORMAT");
			return false;
		}

		hr = Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &brush);

		if FAILED(hr)
		{
			Error("FAILED TO CREATE SOLID COLOR BRUSH");
			return false;
		}

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		text = new Text(L"Testing\nTEST", true, { winSize.width / 2, winSize.height / 2 + 200}, 500);
		texts.push_back(text);

		journal = Journal(winSize.width, winSize.height);

		return true;
	}

	void Render()
	{
		Graphics::Get2DRenderTarget().BeginDraw();

		if (newInformationNotation->GetOpacity() <= 0)
			newInformationNotation->SetVisibility(false);

		else
			newInformationNotation->ReduceOpacity(0.0001f);

		for (auto& image : images)
		{
			if (image->visible)
				image->Draw();
		}

		for (auto& text : texts)
		{
			if (text->visible)
				text->Draw(*textFormat.Get(), *brush.Get());
		}

		journal.Draw();

		Graphics::Get2DRenderTarget().EndDraw();
	}

	void GetInput(int x, int y)
	{
		for (auto& button : buttons)
		{
			if (button.OnClick(x, y) && button.visible)
			{
				Print("CLICKED ON BUTTON");
			}
		}
	}

	void OnEvent()
	{
		switch(Event::GetCurrentEvent())
		{
		case EventType::E_DOWN:
			journal.Slide();
		}
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
				CrossCursor->SetVisibility(true);
				ClueCursor->SetVisibility(false);
				break;

			case CursorType::CLUE:
				currentCursorType = CursorType::CLUE;
				CrossCursor->SetVisibility(false);
				ClueCursor->SetVisibility(true);
				break;
			};
		}
	}
};