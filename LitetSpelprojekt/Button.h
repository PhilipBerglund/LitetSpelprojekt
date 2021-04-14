#pragma once
#include "UI.h"

class Button
{
private:
	D2D1_RECT_F rectangle;
public:
	Button(float x, float y, float width, float height)
	{
		rectangle.left = x - (width / 2);
		rectangle.top = y - (height / 2);
		rectangle.right = x + (width / 2);
		rectangle.bottom = y + (height / 2);
	}

	void DrawFilled(ID2D1RenderTarget& renderTarget, ID2D1Brush* brush) { renderTarget.FillRectangle(&rectangle, brush); }
	void DrawOutlined(ID2D1RenderTarget& renderTarget, ID2D1Brush* brush) { renderTarget.DrawRectangle(&rectangle, brush); }
	bool Intersects(int x, int y)
	{

	}
};