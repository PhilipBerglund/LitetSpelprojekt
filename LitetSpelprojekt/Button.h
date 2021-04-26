#pragma once
#include "UIComponent.h"

class Button :public UIComponent
{
private:
	ComPtr<ID2D1SolidColorBrush> brush;
	bool hovering;
public:
	Button() = default;
	Button(bool visible, D2D_VECTOR_2F position, float width, float height)
		:UIComponent(visible, position, width, height)
	{
		bounds = D2D1::RectF(position.x - (width / 2),
							position.y - (height / 2),
							position.x + (width / 2),
							position.y + (height / 2) );

		Graphics::Get2DRenderTarget().CreateSolidColorBrush(D2D1::ColorF(1, 0, 0), &brush);
	};

	bool OnClick(int x, int y)
	{
		if (x > bounds.left && x < bounds.right && y < bounds.bottom && y > bounds.top)
			return true;
			
		return false;
	};

	bool OnHover(int x, int y)
	{
		if (x > bounds.left && x < bounds.right && y < bounds.bottom && y > bounds.top)
		{
			hovering = true;
			return true;
		}
		
		hovering = false;
		return false;
	}

	void DrawBounds()
	{
		Graphics::Get2DRenderTarget().DrawRectangle(bounds, brush.Get());
	}

	bool IsHovered() { return this->hovering; };
};