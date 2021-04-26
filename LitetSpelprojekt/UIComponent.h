#pragma once
#include "Graphics.h"
#include "ComPtr.h"
#include "Print.h"

struct UIComponent
{
	bool visible;
	float width, height;
	D2D1_RECT_F bounds = {};
	D2D_VECTOR_2F position = {};

	UIComponent() = default;
	UIComponent(bool visible, D2D_VECTOR_2F position, float width = 0, float height = 0)
		:visible(visible), width(width), height(height), position(position)
	{
	}

	void ToggleVisibility()
	{
		if (this->visible)
			visible = false;
		else
			visible = true;
	};

	void SetPosition(float x, float y)
	{
		bounds = {
				x - width / 2,
				y - height / 2,
				x + width / 2,
				y + height / 2
				};
	}

	void SetVisibility(bool value)
	{
		this->visible = value;
	}
};