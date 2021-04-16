#pragma once
#include "UI_Component.h"

class Button
{
private:
	ComPtr<ID2D1RectangleGeometry> rectangle;
	D2D1_RECT_F bounds = D2D1::RectF();
	bool visibility = true;
public:
	Button() = default;
	Button(ID2D1Factory& factory, float x, float y, float width, float height)
	{
		bounds = D2D1::RectF(	x - (width / 2),
								y - (height / 2),
								x + (width / 2),
								y + (height / 2) );

		HRESULT hr = factory.CreateRectangleGeometry(bounds, &rectangle);
		if FAILED(hr)
		{
			Error("FAILED TO CREATE RECTANGLE");
			return;
		}
	};

	void Draw(ID2D1RenderTarget& renderTarget, ID2D1Brush* brush) { renderTarget.DrawGeometry(rectangle.Get(), brush); };
	
	bool IsVisible() { return this->visibility; };

	void ToggleVisibility() 
	{ 
		if (this->visibility)
			visibility = false;
		else
			visibility = true;
	};

	void Move(ID2D1Factory& factory, float x)
	{
		D2D1_RECT_F newBounds = D2D1::RectF(bounds.left + x, bounds.top, bounds.right + x, bounds.bottom);
		bounds = newBounds;

		HRESULT hr = factory.CreateRectangleGeometry(bounds, &rectangle);
		if FAILED(hr)
		{
			Error("FAILED TO CREATE RECTANGLE");
			return;
		}
	}

	bool OnClick(int x, int y)
	{
		if (x > bounds.left && x < bounds.right && y < bounds.bottom && y > bounds.top)
			return true;
			
		return false;
	};
};