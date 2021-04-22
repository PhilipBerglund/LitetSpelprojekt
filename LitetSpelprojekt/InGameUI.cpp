#include "InGameUI.h"

InGameUI::InGameUI()
{
}

bool InGameUI::Initialize(ID2D1RenderTarget& renderTarget)
{
	journalButton = new Image(L"UI/MenuButton.png", renderTarget, 0.9f, true, { 50, 70 });
	pauseMenuButton = new Image(L"UI/JournalButton.png", renderTarget, 0.9f, true, { 60, renderTarget.GetSize().height - 90 });
	newInformationNotation = new Image(L"UI/InformationAdded.png", renderTarget, 0.9f, false, { renderTarget.GetSize().width - 240, 40 });
	journal = new Image(L"UI/Journal.png", renderTarget, 1.0f, false, { renderTarget.GetSize().width - 350, renderTarget.GetSize().height / 2 });
	pauseMenu = new Image(L"UI/PauseMenu.png", renderTarget, 0.9f, false, { renderTarget.GetSize().width / 2, renderTarget.GetSize().height / 2 });

	images.push_back(journalButton);
	images.push_back(pauseMenuButton);
	images.push_back(newInformationNotation);
	images.push_back(journal);
	images.push_back(pauseMenu);

	return true;
}

void InGameUI::Render(ID2D1RenderTarget& renderTarget)
{
	renderTarget.BeginDraw();

	for (auto& image : images)
	{
		if (image->visible)
			image->Draw(renderTarget);
	}
		
	renderTarget.EndDraw();
}

void InGameUI::GetInput(int x, int y)
{
	for (auto& button : buttons)
	{
		if (button.OnClick(x, y) && button.visible)
		{
			Print("CLICKED ON BUTTON");
		}
	}
}

void InGameUI::Notify(InputHandler& input)
{
	if (input.KeyIsPressed('E'))
		journal->ToggleVisibility();
}

void InGameUI::ShowNotification(bool isShowing)
{
	newInformationNotation->visible = isShowing;
}
