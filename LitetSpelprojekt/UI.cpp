#include "UI.h"

HRESULT UI::CreateFactory()
{
	return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &factory);;
}

HRESULT UI::CreateRenderTarget(Graphics& graphics, HWND window)
{
	UINT dpiX;
	UINT dpiY;
	dpiX = dpiY = GetDpiForWindow(window);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

	return factory->CreateDxgiSurfaceRenderTarget(&graphics.GetSurface(), props, &renderTarget);
}

bool UI::Initialize(Graphics& graphics, HWND window)
{
	if FAILED(CreateFactory())
	{
		Error("- FAILED TO CREATE FACTORY -");
		return false;
	}
	
	if FAILED(CreateRenderTarget(graphics, window))
	{
		Error("- FAILED TO CREATE SURFACE RENDER TARGET -");
		return false;
	}

	return true;
}