#pragma once
#include "UI.h"
#include <vector>
#include "InputHandler.h"

enum class CursorType { CROSS, CLUE, CHAT };

class InGameUI
{
private:
	std::vector<Button> buttons;
	std::vector<Image*> images;
	Image* journalButton;
	Image* pauseMenuButton;
	Image* newInformationNotation;
	Image* journal;
	Image* pauseMenu;

	Image* CrossCursor;
	Image* ClueCursor;
	CursorType currentCursorType = CursorType::CROSS;

public:
	InGameUI()
	{
		this->journalButton = nullptr;
		this->pauseMenuButton = nullptr;
		this->newInformationNotation = nullptr;
		this->journal = nullptr;
		this->pauseMenu = nullptr;
		this->CrossCursor = nullptr;
		this->ClueCursor = nullptr;
	};

	bool Initialize()
	{
		D2D1_SIZE_F winSize = Graphics::Get2DRenderTarget().GetSize();

		journalButton = new Image(L"UI/MenuButton.png", 0.9f, true, { 50, 70 });
		pauseMenuButton = new Image(L"UI/JournalButton.png", 0.9f, true, { 60, winSize.height - 90 });
		newInformationNotation = new Image(L"UI/InformationAdded.png", 0.9f, false, { winSize.width - 240, 40 });
		journal = new Image(L"UI/Journal.png", 1.0f, false, { winSize.width - 350, winSize.height / 2 });
		pauseMenu = new Image(L"UI/PauseMenu.png", 0.9f, false, { winSize.width / 2, winSize.height / 2 });
		CrossCursor = new Image(L"UI/CrossCursor.png", 1.0f, true, { winSize.width / 2, winSize.height / 2 });
		ClueCursor = new Image(L"UI/ClueCursor.png", 1.0f, false, { winSize.width / 2, winSize.height / 2 });

		images.push_back(journalButton);
		images.push_back(pauseMenuButton);
		images.push_back(newInformationNotation);
		images.push_back(journal);
		images.push_back(pauseMenu);
		images.push_back(CrossCursor);
		images.push_back(ClueCursor);

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

	void Notify(InputHandler& input)
	{
		if (input.KeyIsPressed('E'))
			journal->ToggleVisibility();
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