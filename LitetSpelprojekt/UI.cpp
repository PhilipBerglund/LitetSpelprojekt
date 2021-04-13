#include "UI.h"

bool UI::Initialize(Graphics& graphics)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &factory)))
		return false;

	IDXGISurface* surface;
	if (FAILED(graphics.GetSwapChain().GetBuffer(0, IID_PPV_ARGS(&surface))))
		return false;
	
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(factory->CreateDxgiSurfaceRenderTarget(surface, props, &renderTarget)))
		return false;

	return true;
}

ID2D1Factory& UI::GetFactory()
{
	return *this->factory.Get();
}

ID2D1RenderTarget& UI::GetRenderTarget()
{
	return *this->renderTarget.Get();
}