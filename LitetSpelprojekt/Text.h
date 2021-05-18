#pragma once
#include "UIComponent.h"

class Text :public UIComponent
{
private:
	std::wstring string;
public:
	Text() = default;

	Text(std::wstring string, bool visible, D2D_VECTOR_2F position, float width = 0, float height = 0)
		:UIComponent(visible, position, width, height), string(string)
	{
		bounds = D2D1::RectF(position.x - (width / 2),
			position.y - (height / 2),
			position.x + (width / 2),
			position.y + (height / 2));
	}

	void SetString(std::wstring string)
	{
		this->string = string;
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		Graphics::Get2DRenderTarget().DrawTextW(string.c_str(), (UINT32)string.size(), &format, bounds, &brush);
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush, UINT32 numChars)
	{
		Graphics::Get2DRenderTarget().DrawTextW(string.c_str(), numChars, &format, bounds, &brush);
	}

	size_t GetSize() const			{ return string.size(); }
	std::wstring GetText() const	{ return string; }
};