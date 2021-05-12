#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <directxmath.h>
#include "Graphics.h"
#include "Light.h"

using namespace DirectX;

struct SceneBounds
{
	XMFLOAT3 center = { 0.0f,0.0f,0.0f };
	float radius = 0;
};

class ShadowMap
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();
	ComPtr<ID3D11ShaderResourceView> DepthMapSRV();
	void BindDsvAndSetNullRenderTarget();
	void UpdateLightAndShadow(Light& light);
	XMFLOAT4X4 GetShadowMatrix()const;
private:
	ShadowMap(const ShadowMap& rhs);
	ShadowMap& operator=(const ShadowMap& rhs);
private:
	UINT mWidth;
	UINT mHeight;
	SceneBounds sceneBounds;
	XMFLOAT4X4 shadowMatrix;
	ComPtr<ID3D11ShaderResourceView> mDepthMapSRV;
	ComPtr<ID3D11DepthStencilView> mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};
