#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxmath.h>


class ShadowMap
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget();
private:
	ShadowMap(const ShadowMap& rhs);
	ShadowMap& operator=(const ShadowMap& rhs);
private:
	UINT mWidth;
	UINT mHeight;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};
