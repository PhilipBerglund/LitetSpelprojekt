#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include "Graphics.h"


class ShadowMap
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();
	ComPtr<ID3D11ShaderResourceView> DepthMapSRV();
	void BindDsvAndSetNullRenderTarget();
	void UpdateLightAndShadow(Light& light);
private:
	ShadowMap(const ShadowMap& rhs);
	ShadowMap& operator=(const ShadowMap& rhs);
private:
	UINT mWidth;
	UINT mHeight;
	ComPtr<ID3D11ShaderResourceView> mDepthMapSRV;
	ComPtr<ID3D11DepthStencilView> mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};