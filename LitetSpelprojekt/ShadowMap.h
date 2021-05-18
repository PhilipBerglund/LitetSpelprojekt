#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include "Graphics.h"
#include "Light.h"

struct SceneBounds
{
	XMFLOAT3 center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float radius = 0.0f;
};

class ShadowMap
{
public:
	ShadowMap();
	ShadowMap(UINT width, UINT height);
	~ShadowMap();
	ID3D11ShaderResourceView** DepthMapSRV();
	ComPtr<ID3D11DepthStencilView> DepthMapDSV();
	void BindDsvAndSetNullRenderTarget();
	void UpdateLightAndShadow(DirectX::XMFLOAT3 lightDir, DirectX::XMFLOAT4X4 shadowMatrix);
private:
	ShadowMap(const ShadowMap& rhs);
	ShadowMap& operator=(const ShadowMap& rhs);
private:
	UINT mWidth;
	UINT mHeight;
	ComPtr<ID3D11ShaderResourceView> mDepthMapSRV;
	ComPtr<ID3D11DepthStencilView> mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
	SceneBounds sceneBounds;
};