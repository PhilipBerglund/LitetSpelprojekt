#pragma once
#include "UI.h"
#include <vector>
#include "InputHandler.h"

class InGameUI
{
	std::vector<Button> buttons;
	std::vector<Image*> images;
	Image* journalButton;
	Image* pauseMenuButton;
	Image* newInformationNotation;
	Image* journal;
	Image* pauseMenu;
public:
	InGameUI();
	bool Initialize(ID2D1RenderTarget& renderTarget);
	void Render(ID2D1RenderTarget& renderTarget);
	void GetInput(int x, int y);
	void Notify(InputHandler& input);
	void ShowNotification(bool isShowing);
};