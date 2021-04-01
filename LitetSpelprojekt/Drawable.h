#pragma once
#include <d3d11.h>

class Drawable
{
protected:
	Drawable() = default;
	virtual void UpdateBuffers(ID3D11DeviceContext* context) = 0;
};