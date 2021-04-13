#pragma once
#include "UI.h"

class MainMenu
{
	ComPtr<ID2D1SolidColorBrush> brush;
	D2D1_RECT_F rectangle;
public:
	MainMenu();
	bool Initialize(ID2D1RenderTarget& renderTarget);
	void Render(ID2D1RenderTarget& renderTarget);
};