#pragma once
#include "UI_Button.h"
#include "UI_Image.h"
#include <vector>

class MainMenu
{
	ComPtr<ID2D1SolidColorBrush> brush;
	std::vector<Button*> buttons;
	std::vector<Button*> visibleComponents;
	Image* image;
public:
	MainMenu();
	bool Initialize(ID2D1Factory& factory, ID2D1RenderTarget& renderTarget);
	void Render(ID2D1Factory& factory, ID2D1RenderTarget& renderTarget);
	void GetInput(int x, int y);
};