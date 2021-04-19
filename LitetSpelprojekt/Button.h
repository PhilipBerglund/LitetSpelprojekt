#pragma once
#include "UIComponent.h"

class Button :public UIComponent
{
public:
	Button() = default;
	Button(bool visible, D2D_VECTOR_2F position, float width, float height)
		:UIComponent(visible, position, width, height)
	{
		bounds = D2D1::RectF(position.x - (width / 2),
							position.y - (height / 2),
							position.x + (width / 2),
							position.y + (height / 2) );
	};

	bool OnClick(int x, int y)
	{
		if (x > bounds.left && x < bounds.right && y < bounds.bottom && y > bounds.top)
			return true;
			
		return false;
	};
};