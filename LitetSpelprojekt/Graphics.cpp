#include "Graphics.h"

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!core.Initialize(windowWidth, windowHeight, window))
	{
		Error("- FAILED TO INITIALIZE D3D11 -");
		return false;
	}

	return true;
}