#pragma once
#include <d2d1.h>
#include "Graphics.h"

class UI
{
private:
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1RenderTarget> renderTarget;

	HRESULT CreateFactory();
	HRESULT CreateRenderTarget(Graphics& graphics, HWND window);
public:
	UI() = default;
	bool Initialize(Graphics& graphics, HWND window);
	/*ID2D1Factory& GetFactory()			{ return *this->factory.Get(); };*/
	ID2D1RenderTarget& GetRenderTarget()	{ return *this->renderTarget.Get(); };
};