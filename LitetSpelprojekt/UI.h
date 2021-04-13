#pragma once
#include <d2d1.h>
#include "Graphics.h"

class UI
{
private:
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1RenderTarget> renderTarget;
	ComPtr<ID2D1Bitmap> bitmap;
public:
	UI() = default;
	bool Initialize(Graphics& graphics);
	ID2D1Factory& GetFactory();
	ID2D1RenderTarget& GetRenderTarget();
};