#pragma once

enum class State { MAINMENU, INGAME, OPENJOURNAL, PAUSED, END };

class MainMenu
{
private:
	std::vector<Button*> buttons;
	std::vector<Image*> images;

	Image* background;

	Image* playButtonArrow;
	Image* optionsButtonArrow;
	Image* helpButtonArrow;
	Image* creditButtonArrow;
	Image* exitButtonArrow;

	Image* cursor;

	Button* playButton;
	Button* optionsButton;
	Button* helpButton;
	Button* creditsButton;
	Button* exitButton;
public:
	MainMenu()
	{
		this->background = nullptr;

		this->playButtonArrow = nullptr;
		this->optionsButtonArrow = nullptr;
		this->helpButtonArrow = nullptr;
		this->creditButtonArrow = nullptr;
		this->exitButtonArrow = nullptr;

		this->playButton = nullptr;
		this->optionsButton = nullptr;
		this->helpButton = nullptr;
		this->creditsButton = nullptr;
		this->exitButton = nullptr;
	};

	bool Initialize()
	{
		D2D1_SIZE_F winSize = Graphics::Get2DRenderTarget().GetSize();

		playButton = new Button(true, { winSize.width / 2 - 5, winSize.height / 2 - 105}, 135.0f, 50.0f);
		optionsButton = new Button(true, { winSize.width / 2 - 5, winSize.height / 2 + 15}, 230.0f, 50.0f);
		helpButton = new Button(true, { winSize.width / 2 - 5, winSize.height / 2 + 150 }, 350.0f, 50.0f);
		creditsButton = new Button(true, { winSize.width / 2, winSize.height / 2 + 260 }, 215.0f, 50.0f);
		exitButton = new Button(true, { winSize.width / 2 - 5, winSize.height / 2 + 380 }, 110.0f, 50.0f);

		background = new Image(L"UI/MainMenu.png", 1.0f, true, { winSize.width / 2, winSize.height / 2 });
		images.push_back(background);

		playButtonArrow = new Image(L"UI/PlayArrows.png", 1.0f, true, { playButton->position.x, playButton->position.y });
		images.push_back(playButtonArrow);

		optionsButtonArrow = new Image(L"UI/OptionsArrows.png", 1.0f, false, { optionsButton->position.x, optionsButton->position.y });
		images.push_back(optionsButtonArrow);

		helpButtonArrow = new Image(L"UI/HelpArrows.png", 1.0f, false, { helpButton->position.x, helpButton->position.y });
		images.push_back(helpButtonArrow);

		creditButtonArrow = new Image(L"UI/CreditsArrows.png", 1.0f, false, { creditsButton->position.x, creditsButton->position.y });
		images.push_back(creditButtonArrow);

		exitButtonArrow = new Image(L"UI/ExitArrows.png", 1.0f, false, { exitButton->position.x, exitButton->position.y });
		images.push_back(exitButtonArrow);

		cursor = new Image(L"UI/RegularCursor.png", 1.0f, true, { winSize.width / 2, winSize.height / 2});
		images.push_back(cursor);

		buttons.push_back(playButton);
		buttons.push_back(optionsButton);
		buttons.push_back(helpButton);
		buttons.push_back(creditsButton);
		buttons.push_back(exitButton);

		return true;
	}

	void Render()
	{
		Graphics::Get2DRenderTarget().BeginDraw();

		for (auto& image : images)
		{
			if (image->visible)
				image->Draw();
		}

		//for (auto& button : buttons)
		//{
		//	if (button->IsHovered())
		//		button->DrawBounds();
		//}

		Graphics::Get2DRenderTarget().EndDraw();
	}

	void GetMousePos(int x, int y)
	{
		cursor->SetPosition(x, y + 30);

		for (int i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i]->OnHover(x, y))
				images[i + 1]->SetVisibility(true);
			else
				images[i + 1]->SetVisibility(false);
		}
	}

	State GetInput(int x, int y)
	{
		if (playButton->OnClick(x, y))
			return State::INGAME;

		if (optionsButton->OnClick(x, y))
			return State::MAINMENU;

		if (helpButton->OnClick(x, y))
			return State::MAINMENU;

		if (creditsButton->OnClick(x, y))
			return State::MAINMENU;

		if (exitButton->OnClick(x, y))
			exit(0);

		return State::MAINMENU;
	}
};